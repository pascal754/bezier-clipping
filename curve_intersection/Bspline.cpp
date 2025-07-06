// September 2, 2022
// by SM
//
// numbered algorithms are from 'The NURBS Book' by Les Piegl and Wayne Tiller, publisher: Springer, ISBN 3-540-55069-0

// May 20, 2023
// searchIntersection does not call itself. use queue for iteration.

module;

#include <SFML/Graphics.hpp>

module Bspline;

import Auxilary;
import std;

bool Bspline::DEBUG{ false };
std::ofstream Bspline::logFile;

std::array<double, Bspline::max_degree + 1> Bspline::basis;

int Bspline::findKnotSpan(double u, int control_point_n) const
{
  if (u < knotVector.front() || u > knotVector.back())
  {
    if (DEBUG) { std::println(logFile, "findKnotSpan() u is outside of range: {}", u); }

    std::println(stderr, "findKnotSpan() u is outside of range: {:.20f}", u);

    throw std::runtime_error("findKnotSpan() u is outside of range");
  }

  if (control_point_n < 0)
  {
    throw std::runtime_error("control_point_n is negative.");
  }

  //algorithm A2.1 FindSpan pp68
  if (u == knotVector[control_point_n + 1]) // special case
    return control_point_n;

  int low{ p_degree };
  int high{ control_point_n + 1 };
  int mid{ (low + high) / 2 };

  while (u < knotVector[mid] || u >= knotVector[mid + 1])
  {
    if (u < knotVector[mid])
      high = mid;
    else
      low = mid;

    mid = (low + high) / 2;
  }

  return mid;
}

bool Bspline::checkNumbers() const
{
  if (controlPoints.size() < 1 || p_degree < 1)
  {
    return false;
  }

  // m = n + p + 1, knotVector.size() -1 == controlPoints.size() -1 + p_degree + 1
  if (knotVector.size() == controlPoints.size() + p_degree + 1)
  {
    return true;
  }

  return false;
}

void Bspline::clear()
{
  controlPoints.clear();
  interpolationPoints.clear();
  knotVector.clear();
  convexHull.clear();
  isConvexHullUpdated = false;
  drawUpdated = false;
}

void Bspline::clearControlPoints()
{
  controlPoints.clear();
  convexHull.clear();
  isConvexHullUpdated = false;
  drawUpdated = false;
}

void Bspline::changeDegree(int degree)
{
  if (degree < 1 || degree > max_degree)
    return;

  p_degree = degree;

  if (!interpolationMode)
  {
    makeKnots();
  }
  convexHull.clear();
  isConvexHullUpdated = false;
  drawUpdated = false;
  if (interpolationMode)
  {
    globalCurveInterpolation();
  }
}

void loadPoints(Bspline& curve1, Bspline& curve2, const std::string& filePathName)
{
  std::ifstream dataFile{ filePathName };
  if (!dataFile.is_open())
  {
    std::println(stderr, "file not found");
    return;
  }
  try
  {
    curve1.clear();
    curve2.clear();

    std::string xCoord;
    std::string yCoord;
    dataFile >> xCoord >> yCoord;
    if (xCoord != "A" || yCoord != "A")
      throw std::runtime_error{ "curve A indicator missing" };

    auto readHeader = [&](Bspline& curve) {
      dataFile >> xCoord >> yCoord;
      if (xCoord == "degree")
        curve.changeDegree(std::stoi(yCoord));
      else
        throw std::runtime_error{ "degree not found" };

      dataFile >> xCoord >> yCoord;
      if (xCoord == "interpolation" && yCoord == "true")
        curve.interpolationMode = true;
      else if (xCoord == "interpolation" && yCoord == "false")
        curve.interpolationMode = false;
      else
        throw std::runtime_error{ "interpolation format wrong" };
      };

    readHeader(curve1);

    bool curveBHeader{};

    auto readData = [&](Bspline& curve) {
      if (curve.interpolationMode)
        curve.addInterpolationPoint(Point{ std::stod(xCoord), std::stod(yCoord) });
      else
        curve.addPointAndKnots(Point{ std::stod(xCoord), std::stod(yCoord) });
      std::println("{} {}", xCoord, yCoord);
      };


    while (dataFile >> xCoord >> yCoord)
    {
      if (xCoord == "B" && yCoord == "B")
      {
        curveBHeader = true;
        break;
      }

      readData(curve1);
    }
    if (!curve1.checkNumbers())
      throw std::runtime_error{ "curve1: checkNumbers() failed" };

    if (!curveBHeader)
      throw std::runtime_error{ "curve B indicator missing" };

    readHeader(curve2);

    while (dataFile >> xCoord >> yCoord)
    {
      readData(curve2);
    }
    if (!curve2.checkNumbers())
      throw std::runtime_error{ "curve2: checkNumbers() failed" };

    std::println("curves data loaded");
  }
  catch (const std::exception& e)
  {
    curve1.clear();
    curve2.clear();
    std::println(stderr, "{}", e.what());
  }
  catch (...)
  {
    curve1.clear();
    curve2.clear();
    std::println(stderr, "other file reading error");
  }
}

void savePoints(const Bspline& curve1, const Bspline& curve2, const std::string& filePathName)
{
  if (!curve1.checkNumbers() || !curve2.checkNumbers())
  {
    std::println("checkNumbers() not satisfied");
    return;
  }

  std::ofstream dataFile{ filePathName };

  if (!dataFile.is_open())
  {
    std::println(stderr, "file error");
    return;
  }

  try
  {
    auto writePoints = [&](const std::vector<Point>& points) {
      for (const auto& p : points)
      {
        std::println(dataFile, "{} {}", p.x, p.y);
      }};

    auto writeCurve = [&](const Bspline& curve) {
      std::println(dataFile, "degree {}", curve.getDegree());

      if (curve.interpolationMode)
      {
        std::println(dataFile, "interpolation true");
        writePoints(curve.interpolationPoints);
      }
      else
      {
        std::println(dataFile, "interpolation false");
        writePoints(curve.controlPoints);
      }};

    std::println(dataFile, "A A");
    writeCurve(curve1);

    std::println(dataFile, "\nB B");
    writeCurve(curve2);

    std::println("points data written to {}", filePathName);
  }
  catch (const std::exception& e)
  {
    std::println(stderr, "{}", e.what());
  }
  catch (...)
  {
    std::println(stderr, "error while saving the file");
  }
}

void Bspline::basisFuns(const Bspline& crv, int i, double u)
{
  // Algorithm A2.2 pp70
  // Compute the nonvanishing basis functions

  static std::array<double, max_degree + 1> left;
  static std::array<double, max_degree + 1> right;

  basis[0] = 1.0;

  for (int j{ 1 }; j <= crv.p_degree; ++j)
  {
    left[j] = u - crv.knotVector[i + 1 - j];
    right[j] = crv.knotVector[i + j] - u;
    double saved{};
    for (int r{}; r < j; ++r)
    {
      double temp{ basis[r] / (right[r + 1] + left[j - r]) };
      basis[r] = saved + right[r + 1] * temp;
      saved = left[j - r] * temp;
    }
    basis[j] = saved;
  }
}

void Bspline::curvePoint(double u, Point& cp) {
  // Algorithm A3.1 pp82
  // result will be saved in cp

  // If all knot values are the same then cp would result in NAN without the following block.
  if (u == knotVector.front() && u == knotVector.back())
  {
    cp = controlPoints.front();
    return;
  }

  int span{ findKnotSpan(u, cp_n()) };
  basisFuns(*this, span, u);

  cp.x = cp.y = 0.0;
  for (int i{}; i <= p_degree; ++i)
  {
    cp.x += basis[i] * controlPoints[span - p_degree + i].x;
    cp.y += basis[i] * controlPoints[span - p_degree + i].y;
  }
}

void Bspline::addPoint(const Point& p) // control point
{
  controlPoints.push_back(p);
  isConvexHullUpdated = false;
  drawUpdated = false;
}

void Bspline::addPointAndKnots(const Point& p) // control point
{
  addPoint(p);
  makeKnots();
}

void Bspline::addInterpolationPoint(const Point& p)
{
  if (interpolationPoints.size() >= 1 && p == interpolationPoints.back())
  {
    std::println(stderr, "The same point cannot be selected in a row.");
    std::println(stderr, "Please, select a different point.");
    return;
  }
  interpolationPoints.push_back(p);
  isConvexHullUpdated = false;
  drawUpdated = false;
  globalCurveInterpolation();
}

void Bspline::deleteLastPoint() // control point
{
  controlPoints.pop_back();
  isConvexHullUpdated = false;
  drawUpdated = false;
}

void Bspline::deleteLastPointAndKnots() // control point
{
  if (controlPoints.size() == 0)
    return;

  deleteLastPoint();
  makeKnots();
}

void Bspline::deleteLastInterpolationPoint()
{
  if (interpolationPoints.size() == 0)
  {
    return;
  }

  interpolationPoints.pop_back();
  isConvexHullUpdated = false;
  drawUpdated = false;
  globalCurveInterpolation();
}

int Bspline::findFirstPointOfConvexHull() const
{
  auto pos{ 0 };

  for (auto i{ 1 }; i < std::ssize(controlPoints); ++i)
  {
    if (controlPoints[i].y < controlPoints[pos].y)
    {
      pos = i;
    }
    else if (controlPoints[i].y == controlPoints[pos].y &&
      controlPoints[i].x < controlPoints[pos].x)
    {
      if (DEBUG)
      {
        std::println(logFile, "findFirstPointOfConvexHull() two points have the same y values");
      }
      pos = i;
    }
  }
  return pos;
}

void Bspline::findConvexHull()
{
  if (isConvexHullUpdated)
  {
    return;
  }

  if (controlPoints.size() <= 1)
  {
    convexHull = controlPoints;
    isConvexHullUpdated = true;
    return;
  }

  convexHull.clear();

  // find the lowest y-coordinate and leftmost point
  const Point& firstCHPoint{ controlPoints[findFirstPointOfConvexHull()] };

  std::list<CHPoint> unsortedPoints;

  for (auto& pt : controlPoints)
  {
    // copy control points except for the first point and the duplicates of it
    if (pt != firstCHPoint)
    {
      // find an angle between the first point of the convex hull
      unsortedPoints.emplace_back(pt.x, pt.y, pt.findAngleAround(firstCHPoint));
    }
  }

  std::list<CHPoint>::iterator min;
  std::list<CHPoint>::iterator index;
  std::vector<Point> sortedPoints;

  // sort points by polar angle around the first point
  // if several points have the same polar angle then keep the farthest point from the first point
  while ((unsortedPoints.size()) >= 2)
  {
    min = unsortedPoints.begin();
    index = std::next(min); // next element
    while (index != unsortedPoints.end())
    {
      if (min->angle > index->angle)
      {
        min = index;
      }
      else if (min->angle == index->angle)
      {
        if (min->p.findDistance(firstCHPoint)
          >= index->p.findDistance(firstCHPoint))
        {
          auto temp{ std::prev(index) };
          unsortedPoints.erase(index);
          index = temp;
        }
        else
        {
          unsortedPoints.erase(min);
          min = index;
        }
      }
      ++index;
    }

    sortedPoints.push_back(min->p);
    unsortedPoints.erase(min);
  }

  if (unsortedPoints.size() == 1)
    sortedPoints.push_back(unsortedPoints.back().p);

  convexHull.push_back(firstCHPoint);

  if (sortedPoints.size() >= 1)
    convexHull.push_back(sortedPoints[0]);

  if (sortedPoints.size() >= 2)
  {
    convexHull.push_back(sortedPoints[1]);
    for (size_t i{ 2 }; i < sortedPoints.size(); ++i) // find convex hull using Graham Scan
    {
      // if not left turn(counterclockwise) then pop the last point
      while (convexHull.size() > 1 &&
        !leftTurn(convexHull[convexHull.size() - 2], convexHull[convexHull.size() - 1], sortedPoints[i]))
      {
        convexHull.pop_back();
      }

      convexHull.push_back(sortedPoints[i]);
    }
  }

  isConvexHullUpdated = true;
} //end findConvexHull

bool Bspline::leftTurn(const Point& p0, const Point& p1, const Point& p2) const
{
  if ((p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y) > 0)
    return true;
  else
    return false;
}


int Bspline::findKnotMult(int i) const
{
  // count multiplicity of knot
  int multiple{ 1 };
  if (i == 0)
    return multiple;

  while (i > 0 && knotVector[i] == knotVector[i - 1])
  {
    ++multiple;
    --i;
  }

  return multiple;
}


bool Bspline::hasEnoughPoints() const
{
  return cp_n() + 1 > p_degree;
}

void Bspline::makeKnots()
{ // make uniform internal knots
    // n + 1 > p
    // n + 1 : number of control points:
    // m: n + 1 + p (last index)
    // p_degree: degree of the curve

  if (!hasEnoughPoints())
  {
    std::println("makeKnots() : not enough control points to make knot vector. add more points.");
    return;
  }

  knotVector.clear();
  knotVector.resize(cp_n() + p_degree + 2); // vector size: m + 1
  for (int i{}; i <= p_degree; ++i)
    knotVector[i] = 0.0;

  int denominator{ cp_n() + 1 - p_degree };

  for (int i{ p_degree + 1 }; i <= cp_n(); ++i)
  {
    knotVector[i] = static_cast<double>(i - p_degree) / denominator;
  }

  int m{ cp_n() + p_degree + 1 };
  for (int i{ cp_n() + 1 }; i <= m; ++i)
    knotVector[i] = 1.0;
}

void Bspline::drawControlPolygon(sf::RenderWindow& window) const
{
  if (cp_n() < 0)
    return;

  sf::VertexArray poly{ sf::PrimitiveType::LineStrip };
  auto wSize{ window.getSize() };
  for (const auto& pt : controlPoints)
    poly.append(sf::Vertex{ sf::Vector2f{ static_cast<float>(pt.x), wSize.y - static_cast<float>(pt.y) } });

  window.draw(poly);
}

void Bspline::drawCurve(sf::RenderWindow& window, sf::Color col, sf::VertexArray& va)
{
  if (checkNumbers())
  {
    findConvexHull();
    if (convexHull.size() == 1) // draw point
    {
      auto wSize{ window.getSize() };
      sf::CircleShape c{ 5 };
      c.setFillColor(col);
      c.setPosition({ static_cast<float>(convexHull.front().x) - c.getRadius(), wSize.y - static_cast<float>(convexHull.front().y) - c.getRadius() });
      window.draw(c);
    }
    else // draw curve
    {
      if (!drawUpdated)
      {
        va.clear();
        double step = (knotVector[cp_n() + 1] - knotVector[0]) / ((cp_n() / 20 + 1) * 200);
        Point pt;
        auto wSize{ window.getSize() };

        for (auto u{ knotVector[0] }; u < knotVector[cp_n() + 1]; u += step)
        {
          curvePoint(u, pt);
          va.append(sf::Vertex{ sf::Vector2f{static_cast<float>(pt.x), wSize.y - static_cast<float>(pt.y)}, col });
        }

        curvePoint(knotVector[cp_n() + 1], pt);
        va.append(sf::Vertex{ sf::Vector2f{static_cast<float>(pt.x), wSize.y - static_cast<float>(pt.y)}, col });
        drawUpdated = true;
      }

      window.draw(va);
    }
  }
  else // draw control or interpolation points until the curve satisfies m = n + p + 1
  {
    auto drawPoints{ [&](std::vector<Point>& points) {
        auto wSize{ window.getSize() };
        for (const auto& p : points)
        {
            sf::CircleShape c{ 5 };
            c.setFillColor(col);
            c.setPosition({ static_cast<float>(p.x) - c.getRadius(), wSize.y - static_cast<float>(p.y) - c.getRadius() });
            window.draw(c);
        }
        } };

    if (interpolationMode)
      drawPoints(interpolationPoints);
    else
      drawPoints(controlPoints);
  }
}

void Bspline::drawConvexHull(sf::RenderWindow& window, sf::Color col)
{
  findConvexHull();
  if (convexHull.size() == 0)
    return;

  sf::VertexArray ch{ sf::PrimitiveType::LineStrip }; //, convexHull.size() + 1};
  auto wSize{ window.getSize() };
  for (const auto& pt : convexHull)
    ch.append(sf::Vertex{ sf::Vector2f{static_cast<float>(pt.x), wSize.y - static_cast<float>(pt.y)}, col });

  // add first point to draw a closed polygon
  ch.append(sf::Vertex{ sf::Vector2f{static_cast<float>(convexHull[0].x), wSize.y - static_cast<float>(convexHull[0].y)}, col });

  window.draw(ch);
}


std::optional<Bspline> Bspline::decompose(double u1, double u2) const
{
  // Algorithm A5.4 RefineKnotVectCurve on pp164

  if (DEBUG) { std::println(logFile, "decompose() u1: {}, u2: {}", u1, u2); }

  if (u1 < 0 || u2 < 0)
  {
    if (DEBUG)
    {
      std::println(logFile, "decompose() negative knot, return");
    }
    return {};
  }

  if (u1 < knotVector.front() || u2 > knotVector.back())
  {
    if (DEBUG)
    {
      std::println(logFile, "decompose() interval is outside the knot vector, return");
    }
    return {};
  }


  if (std::ssize(knotVector) - 1 != cp_n() + p_degree + 1)
  {
    if (DEBUG) { std::println(logFile, "decompose(); m = n + p + 1 not satisfied, return"); }
    return {};
  }

  if (u1 == knotVector[0] && u2 == knotVector[cp_n() + 1])
  {
    if (DEBUG)
    {
      std::println(logFile, "decompose() self decomposition, return");
    }
    return *this;
  }

  // multiplicity for u1 and u2, respectively
  int m1{ p_degree + 1 };
  int m2{ p_degree + 1 };

  // a, b: indices in knotVector
  int pre_a = { findKnotSpan(u1, cp_n()) };
  int pre_b = { findKnotSpan(u2, cp_n()) };

  if (u1 == knotVector[pre_a])
  {
    m1 = findKnotMult(pre_a);
    m1 = p_degree + 1 - m1;
  }

  if (u2 == knotVector[cp_n() + 1])
  {
    m2 = 0;
  }
  else if (pre_a != pre_b && u2 == knotVector[pre_b])
  {
    m2 = findKnotMult(pre_b);
    m2 = p_degree + 1 - m2;
  }

  if (m1 > p_degree + 1 || m1 < 0 || m2 > p_degree + 1 || m2 < 0)
  {
    if (DEBUG)
    {
      std::println(logFile, "decompose() knot vector is wrong");
    }
    return {};
  }

  ++pre_b;

  int sum{ m1 + m2 };
  if (sum == 0)
  {
    if (DEBUG)
    {
      std::println(logFile, "decompose() no knot insertion, return");
    }
    return {};
  }
  std::vector<double> uBar;
  std::vector<Point> qw;

  if (DEBUG) { std::println(logFile, "decompose() pre_a: {}, pre_b: {}, m1: {}, m2: {}", pre_a, pre_b, m1, m2); }


  std::vector<double> X;

  X.insert(X.end(), m1, u1);
  X.insert(X.end(), sum - m1, u2);

  // find new indices for X vector
  int post_a{ findKnotSpan(X.front(), cp_n()) };
  int post_b{ findKnotSpan(X.back(), cp_n()) };

  ++post_b;

  if (DEBUG)
  {
    std::println(logFile, "decompose() post_a: {}, post_b: {}", post_a, post_b);
  }

  int r{ static_cast<int>(std::ssize(X)) - 1 };
  int m{ cp_n() + p_degree + 1 };
  uBar.resize(m + r + 2);
  qw.resize(cp_n() + r + 2);

  for (int j{}; j <= post_a - p_degree; ++j)
  {
    qw[j] = controlPoints[j];
  }
  for (int j{ post_b - 1 }; j <= cp_n(); ++j)
  {
    qw[j + r + 1] = controlPoints[j];
  }
  for (int j{}; j <= post_a; ++j)
  {
    uBar[j] = knotVector[j];
  }
  for (int j{ post_b + p_degree }; j <= m; ++j)
  {
    uBar[j + r + 1] = knotVector[j];
  }

  int i{ post_b + p_degree - 1 };
  int k{ post_b + p_degree + r };

  for (int j{ r }; j >= 0; --j) {
    while (X[j] <= knotVector[i] && i > post_a) {
      qw[k - p_degree - 1] = controlPoints[i - p_degree - 1];
      uBar[k] = knotVector[i];
      --k;
      --i;
    }
    qw[k - p_degree - 1] = qw[k - p_degree];

    for (int L{ 1 }; L <= p_degree; ++L) {
      int ind{ k - p_degree + L };
      double alpha{ uBar[k + L] - X[j] };
      if (alpha == 0.0)
      {
        qw[ind - 1] = qw[ind];
      }
      else
      {
        if (ind >= std::size(qw))
        {
          if (DEBUG)
          {
            std::println(logFile, "decompose() index out of range");
          }
          throw std::runtime_error("decompose() index out of range");
        }

        alpha = alpha / (uBar[k + L] - knotVector[i - p_degree + L]);
        qw[ind - 1].x = alpha * qw[ind - 1].x + (1.0 - alpha) * qw[ind].x;
        qw[ind - 1].y = alpha * qw[ind - 1].y + (1.0 - alpha) * qw[ind].y;
      }
    }
    uBar[k] = X[j];
    --k;
  } // end of algorithm A5.4


  int end{ (u2 == knotVector[cp_n() + 1]) ? pre_b + m1 + p_degree + 1 : pre_b + m1 + m2 };

  int bias{ pre_a + m1 - p_degree };
  std::vector<double> newKnots;

  newKnots.insert(newKnots.end(), uBar.begin() + bias, uBar.begin() + end);


  Bspline decomposed{ p_degree, newKnots };
  size_t num_control_points{ newKnots.size() - p_degree - 1 };

  decomposed.controlPoints.insert(decomposed.controlPoints.end(), qw.begin() + bias, qw.begin() + bias + num_control_points);
  decomposed.id = id;

  if (!decomposed.checkNumbers())
  {
    if (DEBUG)
    {
      std::println(logFile, "decompose() decomposed curve error; m = n + p + 1 not satisfied, return");
    }
    return {};
  }

  return decomposed;
} //end decompose

void Bspline::findLineThruEndPoints()
{
  Point pt0{ controlPoints.front() };
  Point ptn{ controlPoints.back() };
  if (pt0 == ptn)
  {
    if (DEBUG)
    {
      std::println(logFile, "findLineThruEndPoints() first control point coincides with the last one");
    }
    coef_a = 0;
    coef_b = 1;
    coef_c = -pt0.y;
  }
  else
  {
    coef_a = pt0.y - ptn.y;
    coef_b = ptn.x - pt0.x;
    coef_c = -coef_a * pt0.x - coef_b * pt0.y;
    double norm = std::hypot(coef_a, coef_b);
    coef_a /= norm;
    coef_b /= norm;
    coef_c /= norm;
    if (coef_b < 0)
    {
      coef_a = -coef_a;
      coef_b = -coef_b;
      coef_c = -coef_c;
    }
  }
}

void Bspline::findMinMaxDistance()
{
  findLineThruEndPoints();

  Point pt{ controlPoints.front() };

  double dist{ coef_a * pt.x + coef_b * pt.y + coef_c };
  minDist = maxDist = dist;

  for (int i{ 1 }; i < cp_n(); ++i) // the first and the last control point has the same distance
  {
    dist = coef_a * controlPoints[i].x + coef_b * controlPoints[i].y + coef_c;
    minDist = std::fmin(minDist, dist);
    maxDist = std::fmax(maxDist, dist);
  }
}

void Bspline::findMinMaxDistanceFromRotatedLine()
{
  findLineThruEndPoints();

  // rotate clipping lines 90 degrees
  // the line passes through first control point
  double a{ coef_b };
  double b{ -coef_a };
  double c{ -coef_b * controlPoints.front().x + coef_a * controlPoints.front().y };
  double norm{ std::hypot(a, b) };
  a /= norm;
  b /= norm;
  c /= norm;

  if (b < 0)
  {
    a = -a;
    b = -b;
    c = -c;
  }
  coef_a = a;
  coef_b = b;
  coef_c = c;

  double dist{ coef_a * controlPoints.front().x + coef_b * controlPoints.front().y + coef_c };
  minDist = maxDist = dist;

  for (int i{ 1 }; i <= cp_n(); ++i)
  {
    dist = coef_a * controlPoints[i].x + coef_b * controlPoints[i].y + coef_c;
    minDist = std::fmin(minDist, dist);
    maxDist = std::fmax(maxDist, dist);
  }
}

void Bspline::makeDistanceCurve(Bspline& distanceCurve, const Bspline& crv, double& min, double& max)
{
  isConvexHullUpdated = false;

  double d{ coef_a * crv.controlPoints.front().x + coef_b * crv.controlPoints.front().y + coef_c };
  min = max = d;

  distanceCurve.clearControlPoints();

  for (int i{}; i <= crv.cp_n(); ++i)
  {
    double xi{};
    for (int j{ 1 }; j <= crv.p_degree; ++j)
      xi += crv.knotVector[i + j];

    xi /= crv.p_degree;
    d = coef_a * crv.controlPoints[i].x + coef_b * crv.controlPoints[i].y + coef_c;
    distanceCurve.addPoint(Point{ xi, d });
    min = std::fmin(min, d);
    max = std::fmax(max, d);
  }
}

bool exceedsMaximums(int iter, size_t numIntersectionPoints)
{
  if (iter > Bspline::max_iteration)
  {
    std::println("maximum number of iteration reached({}), return", Bspline::max_iteration);
    if (Bspline::DEBUG) { std::println(Bspline::logFile, "maximum number of iteration reached({}), return", Bspline::max_iteration); }
    return true;
  }

  if (std::cmp_greater(numIntersectionPoints, Bspline::max_num_intersection_points))
  {
    std::println("The number of intersection points reached({}), return", Bspline::max_num_intersection_points);
    if (Bspline::DEBUG) { std::println(Bspline::logFile, "The number of intersection points reached({}), return", Bspline::max_num_intersection_points); }
    return true;
  }

  return false;
}

void searchIntersection(std::queue<TwoCurves>& bQueue, ParamInfo& paramInfo)
{
  ++paramInfo.iterationNum;

  if (Bspline::DEBUG)
  {
    std::println(Bspline::logFile, "\n~~~ Iteration #{} ~~~", paramInfo.iterationNum);
  }

  Bspline& crv1{ bQueue.front().c1 };
  Bspline& crv2{ bQueue.front().c2 };

  // necessary when duplicate points were selected in the control point mode
  crv1.checkPointToShrink();
  crv2.checkPointToShrink();

  paramInfo.vNodeInfo.push_back({
      paramInfo.iterationNum,
      bQueue.front().parentIter,
      {crv1.getID(), crv1.knotVector.front(), crv1.knotVector.back()},
      {crv2.getID(), crv2.knotVector.front(), crv2.knotVector.back()},
      ReturnInfo::Default,
      bQueue.front().depth });

  if (Bspline::DEBUG)
  {
    std::println(Bspline::logFile, "queue size: {}", bQueue.size());
    std::println(Bspline::logFile, "curve A, U: [{} {}], deltaU: {}", crv1.knotVector.front(), crv1.knotVector.back(), crv1.knotVector.back() - crv1.knotVector.front());
    std::println(Bspline::logFile, "curve B, U: [{} {}], deltaU: {}", crv2.knotVector.front(), crv2.knotVector.back(), crv2.knotVector.back() - crv2.knotVector.front());
    std::println(Bspline::logFile, "curve A:");
    crv1.printInfo();
    std::println(Bspline::logFile, "curve B:");
    crv2.printInfo();
  }

  // check if maximum numbers are reached
  if (exceedsMaximums(paramInfo.iterationNum, paramInfo.iPoints.size()))
  {
    paramInfo.vNodeInfo.back().rInfo = ReturnInfo::Maximum;
    return;
  }

  double deltaU1{ crv1.knotVector[crv1.cp_n() + 1] - crv1.knotVector[0] };
  double deltaU2{ crv2.knotVector[crv2.cp_n() + 1] - crv2.knotVector[0] };

  if (deltaU1 < 0 || deltaU2 < 0)
  {
    if (Bspline::DEBUG)
    {
      std::println(Bspline::logFile, "searchIntersection() negative delta u, return");
    }
    paramInfo.vNodeInfo.back().rInfo = ReturnInfo::NegativeDelta;
    return;
  }

  if (deltaU1 < Bspline::u_epsilon && deltaU2 < Bspline::u_epsilon)
  {

    if (Bspline::DEBUG)
    {
      std::println(Bspline::logFile, "deltaU1 and deltaU2 are within tolerance: candidate for intersection");
    }

    Point intersectPt;
    Point intersectPt2;

    if (deltaU1 == 0)
    {
      intersectPt = crv1.controlPoints.front();
    }
    else
    {
      crv1.curvePoint(crv1.knotVector[0] + deltaU1 / 2.0, intersectPt);
    }

    if (deltaU2 == 0)
    {
      intersectPt2 = crv2.controlPoints.front();
    }
    else
    {
      crv2.curvePoint(crv2.knotVector[0] + deltaU2 / 2.0, intersectPt2);
    }

    if (Bspline::DEBUG)
    {
      std::println(Bspline::logFile, "u on curve A: {}, u on curve B: {}", crv1.knotVector[0] + deltaU1 / 2.0, crv2.knotVector[0] + deltaU2 / 2.0);
      std::println(Bspline::logFile, "a candidate point on curve A: ({}, {})", intersectPt.x, intersectPt.y);
      std::println(Bspline::logFile, "a candidate point on curve B: ({}, {})", intersectPt2.x, intersectPt2.y);
    }

    if (intersectPt.hasSameCoordWithTolerance(intersectPt2))
    {
      if (Bspline::DEBUG) { std::println(Bspline::logFile, "=== Intersection found === "); }
      paramInfo.iPoints.push_back(intersectPt);
      paramInfo.vNodeInfo.back().rInfo = ReturnInfo::Found;
      return;
    }
    else
    {
      if (Bspline::DEBUG) { std::println(Bspline::logFile, "Two points are not within tolerance. no intersection. continuing..."); }
    }
  }

  if (deltaU1 > deltaU2)
  {
    if (Bspline::DEBUG) { std::println(Bspline::logFile, "switching two curves"); }
    bQueue.push(TwoCurves{ std::move(crv2), std::move(crv1), paramInfo.iterationNum, bQueue.front().depth });
    paramInfo.vNodeInfo.back().rInfo = ReturnInfo::Switched;
    return;
  }

  if (deltaU1 < Bspline::u1_epsilon)
  {
    if (Bspline::DEBUG) { std::println(Bspline::logFile, "one curve becoming a point. continuing ..."); }
  }

  if (deltaU1 < Bspline::u2_epsilon && deltaU2 < Bspline::u2_epsilon)
  {
    if (Bspline::DEBUG) { std::println(Bspline::logFile, "deltaU1 < u2_epsilon and deltaU2 < u2_epsilon. no more iteration. return"); }
    paramInfo.vNodeInfo.back().rInfo = ReturnInfo::TooSmallDelta;
    return;
  }

  crv1.findMinMaxDistance();

  if (Bspline::DEBUG) { std::println(Bspline::logFile, "min, max of clipping lines of curve A: {}, {}", crv1.minDist, crv1.maxDist); }

  if (crv1.maxDist == crv1.minDist)
  {
    if (Bspline::DEBUG) { std::println(Bspline::logFile, "curve A is a line segment"); }
  }

  Bspline distanceCurve{ Bspline(crv2.p_degree, crv2.knotVector) };

  double min{};
  double max{};

  crv1.makeDistanceCurve(distanceCurve, crv2, min, max);
  if (Bspline::DEBUG) { std::println(Bspline::logFile, "minimum and maximum of distance curve: {}, {}", min, max); }

  // check whether two line segments are on the same line
  if (std::abs(crv1.maxDist - crv1.minDist) < Bspline::rotation_epsilon
    && std::abs(max - min) < Bspline::rotation_epsilon
    && std::abs(crv1.minDist - min) < Bspline::rotation_epsilon)
  {
    // recalculate distance from the rotated line by 90 degree
    crv1.findMinMaxDistanceFromRotatedLine();

    // update distance curve
    crv1.makeDistanceCurve(distanceCurve, crv2, min, max);

    if (Bspline::DEBUG)
    {
      std::println(Bspline::logFile, "Two line segments are on the same line rotating clipping lines 90 degrees.");
      std::println(Bspline::logFile, "min, max of clipping lines of curve A: {}, {}", crv1.minDist, crv1.maxDist);
      std::println(Bspline::logFile, "minimum and maximum of distance curve: {}, {}", min, max);
    }
  }

  if (Bspline::DEBUG)
  {
    std::println(Bspline::logFile, "distance curve information:");
    distanceCurve.printInfo();
  }

  // adjust distance curve
  bool adjusted{};
  for (auto& cp : distanceCurve.controlPoints)
  {
    if (cp.y != crv1.minDist && std::abs(cp.y - crv1.minDist) < Bspline::epsilon)
    {
      cp.y = crv1.minDist;
      distanceCurve.isConvexHullUpdated = false;
      adjusted = true;
      if (Bspline::DEBUG) { std::println(Bspline::logFile, "y value of distance curve adjusted to minDist"); }
    }
    else if (cp.y != crv1.maxDist && std::abs(cp.y - crv1.maxDist) < Bspline::epsilon)
    {
      cp.y = crv1.maxDist;
      distanceCurve.isConvexHullUpdated = false;
      adjusted = true;
      if (Bspline::DEBUG) { std::println(Bspline::logFile, "y value of distance curve adjusted to maxDist"); }
    }
  }

  if (adjusted)
  {
    auto comp = [](const Point& a, const Point& b) -> bool {return a.y < b.y; };
    const auto [small, big] = std::ranges::minmax_element(distanceCurve.controlPoints, comp);
    min = small->y;
    max = big->y;

    if (Bspline::DEBUG)
    {
      std::println(Bspline::logFile, "minimum and maximum of distance curve: {}, {}", min, max);
      std::println(Bspline::logFile, "distance curve information:");
      distanceCurve.printInfo();
    }
  }

  if (min > crv1.maxDist || max < crv1.minDist) // outside the clipping lines: no intersection
  {
    if (Bspline::DEBUG) { std::println(Bspline::logFile, "curve B is outside the clipping lines"); }
    paramInfo.vNodeInfo.back().rInfo = ReturnInfo::Outside;
    return;
  }

  distanceCurve.findConvexHull();

  if (distanceCurve.convexHull.size() < 2)
  {
    if (Bspline::DEBUG) { std::println(Bspline::logFile, "distanceCurve.controlPoints.size() < 2"); }
    throw std::runtime_error("distanceCurve.controlPoints.size() < 2");
  }

  Point pt{ distanceCurve.controlPoints.front() };
  Point pt2{ distanceCurve.controlPoints.back() };

  // at least one of two end points are inside the clipping lines: case 1
  if ((crv1.minDist < pt.y && pt.y < crv1.maxDist) || (crv1.minDist < pt2.y && pt2.y < crv1.maxDist))
  {
    if (Bspline::DEBUG) { std::println(Bspline::logFile, "halving curve B"); }

    auto bs1{ crv2.decompose(crv2.knotVector[0], crv2.knotVector[0] + deltaU2 / 2.0) };
    if (bs1)
    {
      bQueue.push(TwoCurves{ std::move(*bs1), crv1, paramInfo.iterationNum, bQueue.front().depth + 1 });
      if (Bspline::DEBUG) { std::println(Bspline::logFile, "a pair of curves added to bQueue from case 1-1"); }
    }

    auto bs2{ crv2.decompose(crv2.knotVector[0] + deltaU2 / 2.0, crv2.knotVector[crv2.cp_n() + 1]) };
    if (bs2) {
      bQueue.push(TwoCurves{ std::move(*bs2), std::move(crv1), paramInfo.iterationNum, bQueue.front().depth + 1 });
      if (Bspline::DEBUG) { std::println(Bspline::logFile, "a pair of curves added to bQueue from case 1-2"); }
    }

    paramInfo.vNodeInfo.back().rInfo = ReturnInfo::DecomposedC1;
    return;
  }

  // two end points are outside the clipping lines

  std::vector<double> uValues;

  auto findUs = [&](double distance)
    {
      if (crv1.isThereLineIntersection(distance, pt.y, pt2.y))
      {
        if (Bspline::DEBUG) { std::println(Bspline::logFile, "intersection between clipping line and one edge of convex hull of the distance curve: "); }
        if (pt.y == pt2.y)
        {
          if (Bspline::DEBUG)
          {
            std::println(Bspline::logFile, "   the edge is horizontal (distance curve), u values: ");
            std::println(Bspline::logFile, "{} {}", pt.x, pt2.x);
          }
          uValues.push_back(pt.x);
          uValues.push_back(pt2.x);
        }
        else
        {
          uValues.push_back((pt2.x - pt.x) * (distance - pt.y) / (pt2.y - pt.y) + pt.x);
          if (Bspline::DEBUG) { std::println(Bspline::logFile, "   u: {}", uValues.back()); }
        }
      }
    }; // end of findUs

  for (size_t i{}; i < distanceCurve.convexHull.size() - 1; ++i)
  {
    pt = distanceCurve.convexHull[i];
    pt2 = distanceCurve.convexHull[i + 1];
    findUs(crv1.maxDist);
    findUs(crv1.minDist);
  }

  pt = pt2;
  pt2 = distanceCurve.convexHull.front();
  findUs(crv1.maxDist);
  findUs(crv1.minDist);

  if (uValues.size() < 1)
  {
    if (Bspline::DEBUG) { std::println(Bspline::logFile, "the number of clipped u values < 1, returning"); }
    paramInfo.vNodeInfo.back().rInfo = ReturnInfo::NotClipped;
    return;
  }

  auto [u_min, u_max] { std::ranges::minmax(uValues) };

  if (std::isnan(u_min) || std::isnan(u_max))
  {
    if (Bspline::DEBUG) { std::println(Bspline::logFile, "u value is NaN, returning"); }
    paramInfo.vNodeInfo.back().rInfo = ReturnInfo::NaN;
    return;
  }

  if (std::isinf(u_min) || std::isinf(u_max))
  {
    if (Bspline::DEBUG) { std::println(Bspline::logFile, "u value is Inf, returning"); }
    paramInfo.vNodeInfo.back().rInfo = ReturnInfo::Inf;
    return;
  }

  if (u_max == u_min)
  {
    if (Bspline::DEBUG) { std::println(Bspline::logFile, "The interval of the u on the clipped curve is zero: a point"); }
  }

  if (Bspline::DEBUG) { std::println(Bspline::logFile, "u_min: {}, u_max: {}", u_min, u_max); }

  if (u_min < crv2.knotVector.front())
  {
    if (u_max == u_min || u_max < crv2.knotVector.front())
    {
      u_max = crv2.knotVector.front();
      if (Bspline::DEBUG) { std::println(Bspline::logFile, "u_max adjusted to u_0"); }
    }

    u_min = crv2.knotVector.front(); // due to calculation error u_min could be smaller than u_0

    if (Bspline::DEBUG) { std::println(Bspline::logFile, "u_min adjusted to u_0"); }
  }

  if (u_max > crv2.knotVector.back())
  {
    if (u_min == u_max || u_min > crv2.knotVector.back())
    {
      u_min = crv2.knotVector.back();
      if (Bspline::DEBUG) { std::println(Bspline::logFile, "u_min adjusted to u_m"); }
    }
    u_max = crv2.knotVector.back(); // due to calculation error u_max could be slightly larger than u_m

    if (Bspline::DEBUG) { std::println(Bspline::logFile, "u_max adjusted to u_m"); }
  }

  if ((u_max - u_min) > deltaU2 * 0.8) // case 2
  {
    if (Bspline::DEBUG) { std::println(Bspline::logFile, "halving curve B"); }
    auto bs1{ crv2.decompose(crv2.knotVector[0], crv2.knotVector[0] + deltaU2 / 2.0) };
    if (bs1)
    {
      bQueue.push(TwoCurves{ std::move(*bs1), crv1, paramInfo.iterationNum, bQueue.front().depth + 1 });
      if (Bspline::DEBUG) { std::println(Bspline::logFile, "a pair of curves added to bQueue from case 2-1"); }
    }

    auto bs2{ crv2.decompose(crv2.knotVector[0] + deltaU2 / 2.0, crv2.knotVector[crv2.cp_n() + 1]) };
    if (bs2) {
      bQueue.push(TwoCurves{ std::move(*bs2), std::move(crv1), paramInfo.iterationNum, bQueue.front().depth + 1 });
      if (Bspline::DEBUG) { std::println(Bspline::logFile, "a pair of curves added to bQueue from case 2-2"); }
    }

    paramInfo.vNodeInfo.back().rInfo = ReturnInfo::DecomposedC2;
    return;
  }
  else // case 3
  {
    if (Bspline::DEBUG) { std::println(Bspline::logFile, "decomposing curve B between {}, {}", u_min, u_max); }
    auto decomposedC2{ crv2.decompose(u_min, u_max) };
    if (decomposedC2)
    {
      bQueue.push(TwoCurves{ std::move(*decomposedC2), std::move(crv1), paramInfo.iterationNum, bQueue.front().depth + 1 });
      if (Bspline::DEBUG) { std::println(Bspline::logFile, "a pair of curves added to bQueue from case 3"); }
    }

    paramInfo.vNodeInfo.back().rInfo = ReturnInfo::DecomposedC3;
    return;
  }
} //end searchIntersection

void Bspline::checkPointToShrink()
// if a curve is a point then shrink knot vector
{
  findConvexHull();

  if (convexHull.size() != 1)
    return;

  if (knotVector[cp_n() + 1] - knotVector[0] < u2_epsilon)
    return;

  if (DEBUG) { std::println(logFile, "checkPointToShrink(): The curve is a point and the original knot vector interval [{}, {}]", knotVector[0], knotVector[cp_n() + 1]); }

  double u1{ knotVector[0] };
  double u2{ u1 + u2_epsilon / 10.0 };
  auto decomp{ decompose(u1, u2) };
  if (decomp)
  {
    *this = std::move(*decomp);
    if (DEBUG) { std::println(logFile, "checkPointToShrink(): The knot vector shrunk to [{}, {}]", u1, u2); }
  }
}

void findIntersection(Bspline crv1, Bspline crv2, ParamInfo& paramInfo)
{
  try
  {
    if (!crv1.checkNumbers() || !crv2.checkNumbers())
    {
      std::println("m = n + p + 1 not satisfied");
      return;
    }

    crv1.interpolationPoints.clear();
    crv2.interpolationPoints.clear();
    crv1.checkPointToShrink();
    crv2.checkPointToShrink();

    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    std::queue<TwoCurves> bQueue;

    if (paramInfo.decomposeFirst)
    {
      predecomposeCurves(std::move(crv1), std::move(crv2), bQueue);
    }
    else
    {
      bQueue.push(TwoCurves{ std::move(crv1), std::move(crv2), 0, 0 });
    }

    while (!bQueue.empty() && paramInfo.iterationNum < Bspline::max_iteration && paramInfo.iPoints.size() < Bspline::max_num_intersection_points)
    {
      searchIntersection(bQueue, paramInfo);
      bQueue.pop();
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    std::println("\n-------------------------------------------------------");

    if (!Bspline::DEBUG)
    {
      std::println("\n\tTime elapsed: {} microseconds", duration.count());
    }

    printResult(paramInfo.iterationNum, paramInfo.iPoints, Bspline::DEBUG, Bspline::logFile);

    writeNodeInfo(paramInfo.vNodeInfo);
  }
  catch (const std::exception& e)
  {
    std::println(stderr, "{}", e.what());
  }
}

// decompose each curve on knot and find intersection for combination
void predecomposeCurves(Bspline crv1, Bspline crv2, std::queue<TwoCurves>& bQueue)
{
  std::vector<std::optional<Bspline>> bezierList1;
  std::vector<std::optional<Bspline>> bezierList2;

  auto preDecompose = [](Bspline& curve, std::vector<std::optional<Bspline>>& bList) {
    curve.findConvexHull();

    if (curve.convexHull.size() == 1)
    {
      bList.push_back(std::move(curve));
    }
    else
    {
      for (int i{ curve.p_degree }; i <= curve.cp_n(); ++i)
      {
        bList.push_back(curve.decompose(curve.knotVector[i], curve.knotVector[i + 1]));
      }
    }
    };

  preDecompose(crv1, bezierList1);
  preDecompose(crv2, bezierList2);

  if (Bspline::DEBUG) { std::println(Bspline::logFile, "\nthe number of pre-decompositions: {}", bezierList1.size() + bezierList2.size()); }

  for (const auto& b1 : bezierList1) {
    for (const auto& b2 : bezierList2)
    {
      if (b1.has_value() && b2.has_value())
      {
        bQueue.push(TwoCurves{ *b1, *b2, 0, 0 });
      }
    }
  }
}


bool Bspline::isThereLineIntersection(double y, double y1, double y2) const
{
  if ((y1 > y && y2 > y) || (y1 < y && y2 < y))
    return false;
  else
    return true;
}

void Bspline::printInfo() // debug only
{
  findConvexHull();

  std::println(logFile, "---start curve info----------------------------------------------------------------------");
  std::println(logFile, "degree: {}", p_degree);

  std::println(logFile, "knot vector");
  for (const auto& x : knotVector)
    std::print(logFile, " {}", x);

  std::println(logFile, "\nnumber of control points: {}", cp_n() + 1);

  for (auto&& [index, pt] : std::ranges::views::enumerate(std::as_const(controlPoints)))
  {
    std::println(logFile, "{}\t{}", index + 1, pt);
  }

  if (!convexHull.empty()) {
    std::println(logFile, "Convex Hull");
    for (const auto& pt : convexHull)
      std::print(logFile, "   {}", pt);
  }

  std::println(logFile, "\n---end curve info------------------------------------------------------------------------");
}

void Bspline::globalCurveInterpolation()
{
  // Algorithm A9.1 pp. 369
  // Global interpolation through n + 1 points
  // Input: n, Q
  //     n: Q_0 ~ Q_n
  //     Q: interpolation points
  //
  // Output: U, P
  //     U: knot vector
  //     P: control points (solution)

  // interpolationPoints.size() + p_degree  < (p_degree + 1) * 2
  if (std::ssize(interpolationPoints) < p_degree + 2)
  {
    controlPoints.clear();
    std::println(stderr, "No. of interpolation points not enough. Add more points");
    return;
  }

  // 1. compute u_bar_k
  std::vector<double> u_bar_k;
  find_u_bar_k(u_bar_k);

  // 2. compute U
  find_U(u_bar_k);

  // 3. initialize array A to 0
  const auto N{ static_cast<int>(std::ssize(interpolationPoints)) };
  std::vector<std::vector<double>> A(N, std::vector<double>(N, 0.));

  auto vectorAssign = [&](int i, int offset) {
    for (int col{ 0 }; col <= p_degree; ++col)
    {
      A[i][col + offset] = basis[col];
    }};

  for (int i{}; i < N; ++i)
  {
    // Set up coefficient matrix
    int span{};
    try
    {
      span = findKnotSpan(u_bar_k[i], N - 1);
    }
    catch (const std::exception& e)
    {
      controlPoints.clear();
      std::print(stderr, "{}", e.what());
      std::println(stderr, ": global interpolation failed.");
      return;
    }
    basisFuns(*this, span, u_bar_k[i]);
    vectorAssign(i, span - p_degree);
  }

  std::vector<int> Pm(N + 1); // permutation matrix

  if (!LUPDecompose(A, Pm))
  {
    std::println(stderr, "LUDecomposition failed");
    controlPoints.clear();
    return;
  }

  LUPSolve(A, Pm);
}

void Bspline::find_u_bar_k(std::vector<double>& u_bar_k)
{
  // Output: u_bar_k

  // chord length method: Eq. (9.4), (9.5)
  if (interpolationPoints.size() <= 2)
  {
    std::println(stderr, "find_u_bar_k: interpolation points are not enough");
    return;
  }

  double d{};
  for (size_t i{ 1 }; i < interpolationPoints.size(); ++i)
  {
    d += std::hypot(interpolationPoints[i].x - interpolationPoints[i - 1].x, interpolationPoints[i].y - interpolationPoints[i - 1].y);
  }

  u_bar_k.resize(interpolationPoints.size());

  u_bar_k.front() = 0.0;
  u_bar_k.back() = 1.0;

  for (size_t i{ 1 }; i < interpolationPoints.size() - 1; ++i)
  {
    u_bar_k[i] = u_bar_k[i - 1] + std::hypot(interpolationPoints[i].x - interpolationPoints[i - 1].x, interpolationPoints[i].y - interpolationPoints[i - 1].y) / d;
    if (u_bar_k[i] > 1.0)
    {
      std::println(stderr, "u_bar_k is larger > 1.0: {:.20f}", u_bar_k[i]);
    }
  }
}

void Bspline::find_U(const std::vector<double>& u_bar_k)
{
  // Input: u_bar_k
  // Output: change knotVector
  // Eq. (9.8)

  size_t n{ interpolationPoints.size() };
  size_t m{ n + p_degree };

  knotVector.resize(m + 1);

  for (size_t i{}; i <= p_degree; ++i)
  {
    knotVector[i] = 0.0;
  }

  for (size_t i{ m - p_degree }; i <= m; ++i)
  {
    knotVector[i] = 1.0;
  }

  for (size_t j{ 1 }; j < n - p_degree; ++j)
  {
    double sum{};
    for (size_t i{ j }; i <= j + p_degree - 1; ++i)
    {
      sum += u_bar_k[i];
    }
    knotVector[j + p_degree] = sum / p_degree;
  }
}

bool Bspline::LUPDecompose(std::vector<std::vector<double>>& A, std::vector<int>& Pm)
{
  //bool LUPDecompose(vedouble **A, int N, double Tol, int* P)
  // https://en.wikipedia.org/wiki/LU_decomposition
  /* INPUT: A - array of pointers to rows of a square matrix having dimension N

   * OUTPUT: Matrix A is changed, it contains a copy of both matrices L-E and U as A=(L-E)+U such that P*A=L*U.
   *        The permutation matrix is not stored as a matrix, but in an integer vector P of size N+1
   *        containing column indexes where the permutation matrix has "1". The last element P[N]=S+N,
   *        where S is the number of row exchanges needed for determinant computation, det(P)=(-1)^S
   *
   * Tol: small tolerance number to detect failure when the matrix is near degenerate
   */

  size_t N{ interpolationPoints.size() };

  constexpr double Tol{ 1e-6 };

  for (int i{}; i <= N; ++i)
  {
    Pm[i] = i;
  } //Unit permutation matrix, P[N] initialized with N

  for (size_t i{}; i < N; ++i)
  {
    double maxA{};
    size_t iMax{ i };

    double absA{};
    for (size_t k{ i }; k < N; ++k)
    {
      if ((absA = std::abs(A[k][i])) > maxA)
      {
        maxA = absA;
        iMax = k;
      }
    }

    if (maxA < Tol) { return false; } //failure, matrix is degenerate

    if (iMax != i)
    {
      //pivoting P
      std::swap(Pm[i], Pm[iMax]);

      //pivoting rows of A
      std::swap(A[i], A[iMax]);

      //counting pivots starting from N (for determinant)
      ++Pm[N];
    }

    for (size_t j{ i + 1 }; j < N; ++j)
    {
      A[j][i] /= A[i][i];

      for (size_t k{ i + 1 }; k < N; ++k)
      {
        A[j][k] -= A[j][i] * A[i][k];
      }
    }
  }

  return true;  //decomposition done 
}

void Bspline::LUPSolve(const std::vector<std::vector<double>>& A, const std::vector<int>& Pm)
{
  // https://en.wikipedia.org/wiki/LU_decomposition
  /* INPUT: A,P filled in LUPDecompose; b(interpolation points) - rhs vector; N - dimension
   * OUTPUT: x(control points) - solution vector of A*x=b
   */

  const auto N{ std::ssize(A) };
  controlPoints.resize(N);

  for (std::ptrdiff_t i{}; i < N; ++i)
  {
    controlPoints[i].y = interpolationPoints[Pm[i]].y;
    controlPoints[i].x = interpolationPoints[Pm[i]].x;

    for (std::ptrdiff_t k{}; k < i; ++k)
    {
      controlPoints[i].x -= A[i][k] * controlPoints[k].x;
      controlPoints[i].y -= A[i][k] * controlPoints[k].y;
    }
  }

  for (std::ptrdiff_t i{ N - 1 }; i >= 0; --i)
  {
    for (std::ptrdiff_t k{ i + 1 }; k < N; ++k)
    {
      controlPoints[i].x -= A[i][k] * controlPoints[k].x;
      controlPoints[i].y -= A[i][k] * controlPoints[k].y;
    }

    controlPoints[i].x /= A[i][i];
    controlPoints[i].y /= A[i][i];
  }
}
