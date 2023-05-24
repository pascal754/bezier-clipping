// September 2, 2022
// by SM
//
// numbered algorithms are from 'The NURBS Book' by Les Piegl and Wayne Tiller, publisher: Springer, ISBN 3-540-55069-0

// May 20, 2023
// searchIntersection does not call itself. use queue for interation.

module;

#include <SFML/Graphics.hpp>

module Bspline;

import <iostream>;
import <string>;
import <vector>;
import <list>;
import <queue>;
import <utility>;
import <algorithm>;
import <format>;
import <stdexcept>;
import <fstream>;
import <numeric>;
import Auxilary;

bool Bspline::DEBUG{ false };
const double Bspline::epsilon{ 1e-9 }; // epsilon is for approximate zero and should be much less than u_epsilon
const double Bspline::u_epsilon{ 0.0001 }; // for knot values
const double Bspline::u1_epsilon{ u_epsilon / 10.0 };
const double Bspline::u2_epsilon{ u1_epsilon / 10.0 };
const int Bspline::max_iteration{ 500'000 }; // maximum iteration for overlapping curves
const int Bspline::max_num_intersection_points{ 500'000 };
std::ofstream Bspline::logFile;

int Bspline::findKnotSpan(double u) const
{
    if (u < knotVector.front() || u > knotVector.back())
    {
        if (DEBUG) { logFile << "findKnotSpan() u is outside of range: " << u << '\n'; }

        std::cerr << std::format("findKnotSpan() u is outside of range: {:.20f}\n", u);

        throw std::runtime_error("findKnotSpan() u is outside of range");
    }

    //algorithm A2.1 FindSpan pp68
    if (u == knotVector[cp_n + 1]) // special case
        return cp_n;

    int low{ p_degree };
    int high{ cp_n + 1 };
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

    if (knotVector.size() == controlPoints.size() + p_degree + 1) // m = n + p + 1, knotVector.size() -1 == controlPoints.size() -1 + p_degree + 1
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
    cp_n = -1;
}

void Bspline::clearControlPoints()
{
    controlPoints.clear();
    convexHull.clear();
    isConvexHullUpdated = false;
    cp_n = -1;
}

void Bspline::changeDegree(int degree)
{
    if (degree < 1 || degree > 10)
        return;

    p_degree = degree;
    basis.resize(p_degree + 1);
    left.resize(p_degree + 1);
    right.resize(p_degree + 1);

    if (!interpolationMode)
    {
        makeKnots();
    }
    convexHull.clear();
    isConvexHullUpdated = false;
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
        std::cerr << "file not found\n";
        return;
    }
    try
    {
        curve1.clear();
        curve2.clear();

        std::string xCoord, yCoord;
        dataFile >> xCoord >> yCoord;
        if (xCoord != "A" || yCoord != "A")
            throw std::exception("file data wrong");
        
        auto readHeader = [&](Bspline& curve) {
            dataFile >> xCoord >> yCoord;
            if (xCoord == "interpolation" && yCoord == "true")
                curve.interpolationMode = true;
            else
                curve.interpolationMode = false;
            };

        readHeader(curve1);

        bool curveBHeader{};

        auto readData = [&](Bspline& curve) {
            if (curve.interpolationMode)
                curve.addInterpolationPoint(Point{ std::stod(xCoord), std::stod(yCoord) });
            else
                curve.addPointAndKnots(Point{ std::stod(xCoord), std::stod(yCoord) });
            std::cout << xCoord << ' ' << yCoord << '\n';
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
            throw std::exception("curve1: checkNumbers() failed");

        if (curve1.interpolationMode)
            curve1.globalCurveInterpolation();

        if (!curveBHeader)
            throw std::exception("file data error");

        readHeader(curve2);

        while (dataFile >> xCoord >> yCoord)
        {
            readData(curve2);
        }
        if (!curve2.checkNumbers())
            throw std::exception("curve2: checkNumbers() failed");

        if (curve2.interpolationMode)
            curve2.globalCurveInterpolation();

        std::cout << "curves data loaded\n";
    }
    catch (const std::exception& e)
    {
        curve1.clear();
        curve2.clear();
        std::cerr << e.what() << '\n';
    }
    catch (...)
    {
        curve1.clear();
        curve2.clear();
        std::cerr << "file reading error\n";
    }
}

void savePoints(const Bspline& curve1, const Bspline& curve2, const std::string& filePathName)
{
    if (!curve1.checkNumbers() || !curve2.checkNumbers())
    {
        std::cerr << "checkNumbers() not satisfied\n";
        return;
    }

    std::ofstream dataFile{ filePathName };

    if (!dataFile.is_open())
    {
        std::cerr << "file error\n";
        return;
    }

    try
    {
        auto writePoints = [&](const std::vector<Point>& points) {
            for (const auto& p : points)
            {
                dataFile << p.x << ' ' << p.y << '\n';
            }};

        auto writeCurve = [&](const Bspline& curve) {
            if (curve.interpolationMode)
            {
                dataFile << "interpolation true\n";
                writePoints(curve.interpolationPoints);
            }
            else
            {
                dataFile << "interpolation false\n";
                writePoints(curve.controlPoints);
            }};

        dataFile << "A A\n";
        writeCurve(curve1);

        dataFile << "\nB B\n";
        writeCurve(curve2);

        std::cout << "points data written to " << filePathName << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "error while saving the file\n";
    }
}

void Bspline::basisFuns(int i, double u)
{
    // Algorithm A2.2 pp70
    // Compute the nonvanishing basis functions
    double temp, saved;

    basis[0] = 1.0;

    for (int j{ 1 }; j <= p_degree; ++j)
    {
        left[j] = u - knotVector[i + 1 - j];
        right[j] = knotVector[i + j] - u;
        saved = 0.0;
        for (int r{}; r < j; ++r)
        {
            temp = basis[r] / (right[r + 1] + left[j - r]);
            basis[r] = saved + right[r + 1] * temp;
            saved = left[j - r] * temp;
        }
        basis[j] = saved;
    }
}

void Bspline::curvePoint(double u, Point& cp) {
    // Algorithm A3.1 pp82
    // result will be saved in cp

    int span{ findKnotSpan(u) };
    basisFuns(span, u);

    cp.x = cp.y = 0.0;
    for (int i{}; i <= p_degree; ++i) {
        cp.x += basis[i] * controlPoints[span - p_degree + i].x;
        cp.y += basis[i] * controlPoints[span - p_degree + i].y;
    }
}

void Bspline::addPoint(const Point& p) // control point
{
    controlPoints.push_back(p);
    cp_n = static_cast<int>(std::ssize(controlPoints)) - 1;
    isConvexHullUpdated = false;
}

void Bspline::addPointAndKnots(const Point& p) // control point
{
    addPoint(p);
    makeKnots();
}

void Bspline::addInterpolationPoint(const Point& p)
{
    interpolationPoints.push_back(p);
    cp_n = static_cast<int>(std::ssize(interpolationPoints)) - 1;
    isConvexHullUpdated = false;
    globalCurveInterpolation();
}

void Bspline::deleteLastPoint() // control point
{
    controlPoints.pop_back();
    cp_n = static_cast<int>(std::ssize(controlPoints)) - 1;
    isConvexHullUpdated = false;
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
    cp_n = static_cast<int>(std::ssize(interpolationPoints)) - 1;
    isConvexHullUpdated = false;
    convexHull.clear();
    globalCurveInterpolation();
}

Point Bspline::getPoint(int i) const
{
    return controlPoints[i];
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
                logFile << "findFirstPointOfConvexHull() two points have the same y values\n";
            }
            pos = i;
        }
    }
    return pos;
}

void Bspline::findConvexHull()
{
    if (isConvexHullUpdated || controlPoints.size() == 0)
    {
        return;
    }

    if (controlPoints.size() == 1)
    {
        convexHull = controlPoints;
        isConvexHullUpdated = true;
        return;
    }

    convexHull.clear();

    std::list<Point> unsortedPoints;

    // find the lowest y-coordinate and leftmost point
    int firstCHPoint{ findFirstPointOfConvexHull() };

    // find angle between the first point of the convex hull and copy control points except for the first point
    for (size_t i{}; i < controlPoints.size(); ++i) {
        controlPoints[i].findAngleAround(controlPoints[firstCHPoint]);
        if (i != firstCHPoint && !controlPoints[i].hasSameCoord(controlPoints[firstCHPoint])) // do not include the first point of convex hull and the duplicates of it
            unsortedPoints.push_back(controlPoints[i]);
    }

    std::list<Point>::iterator min, index;
    std::vector<Point> sortedPoints, tempConvexHull;

    // sort points by polar angle between the first point
    // if several points have the same polar angle then keep the farthest point from the first point
    while ((unsortedPoints.size()) >= 2) {
        min = unsortedPoints.begin();
        index = std::next(min); // next element
        while (index != unsortedPoints.end()) {
            if (min->angle > index->angle) {
                min = index;
            }
            else if (min->angle == index->angle)
            {
                if (min->findDistance(controlPoints[firstCHPoint]) >= index->findDistance(controlPoints[firstCHPoint]))
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

        sortedPoints.push_back(*min);
        unsortedPoints.erase(min);
    }

    if (unsortedPoints.size() == 1)
        sortedPoints.push_back(unsortedPoints.back());

    tempConvexHull.push_back(controlPoints[firstCHPoint]);

    if (sortedPoints.size() >= 1)
        tempConvexHull.push_back(sortedPoints[0]);

    if (sortedPoints.size() >= 2)
    {
        tempConvexHull.push_back(sortedPoints[1]);
        for (size_t i{ 2 }; i < sortedPoints.size(); ++i) // find convex hull using Graham Scan
        {
            // if not left turn(counterclockwise) then pop the last point
            while (tempConvexHull.size() > 1 &&
                !leftTurn(tempConvexHull[tempConvexHull.size() - 2], tempConvexHull[tempConvexHull.size() - 1], sortedPoints[i]))
            {
                tempConvexHull.pop_back();
            }

            tempConvexHull.push_back(sortedPoints[i]);
        }
    }

    convexHull = tempConvexHull;
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
    return (cp_n + 1 > p_degree) ? true : false;
}

void Bspline::makeKnots()
{ // make uniform internal knots
    // n + 1 > p
    // n + 1 : number of control points:
    // m: n + 1 + p (last index)
    // p_degree: degree of the curve

    if (!hasEnoughPoints())
    {
        std::cout << "makeKnots() : not enough control points to make knot vector. add more points.\n";
        return;
    }

    knotVector.clear();
    knotVector.resize(cp_n + p_degree + 2); // vector size: m + 1
    for (int i{}; i <= p_degree; ++i)
        knotVector[i] = 0.0;

    int denominator{ cp_n + 1 - p_degree };

    for (int i{ p_degree + 1 }; i <= cp_n; ++i)
    {
        knotVector[i] = static_cast<double>(i - p_degree) / denominator;
    }

    int m{ cp_n + p_degree + 1 };
    for (int i{ cp_n + 1 }; i <= m; ++i)
        knotVector[i] = 1.0;
}

void Bspline::drawControlPolygon(sf::RenderWindow& window) const
{
    if (cp_n < 0)
        return;

    sf::VertexArray poly{ sf::LineStrip };
    auto wSize{ window.getSize() };
    for (const auto& pt : controlPoints)
        poly.append(sf::Vertex{ sf::Vector2f{ static_cast<float>(pt.x), wSize.y - static_cast<float>(pt.y) } });

    window.draw(poly);
}

void Bspline::drawCurve(sf::RenderWindow& window, sf::Color col)
{
    if (checkNumbers())
    {
        findConvexHull();
        if (convexHull.size() == 1) // draw point
        {
            auto wSize{ window.getSize() };
            sf::CircleShape c{ 5 };
            c.setFillColor(col);
            c.setPosition(static_cast<float>(convexHull.front().x) - c.getRadius(), wSize.y - static_cast<float>(convexHull.front().y) - c.getRadius());
            window.draw(c);
        }
        else // draw curve
        {
            double step = (knotVector[cp_n + 1] - knotVector[0]) / ((cp_n / 20 + 1) * 200);
            Point pt;
            sf::VertexArray curve{ sf::LineStrip };
            auto wSize{ window.getSize() };

            for (auto u{ knotVector[0] }; u < knotVector[cp_n + 1]; u += step)
            {
                curvePoint(u, pt);
                curve.append(sf::Vertex{ sf::Vector2f{static_cast<float>(pt.x), wSize.y - static_cast<float>(pt.y)}, col });
            }

            curvePoint(knotVector[cp_n + 1], pt);
            curve.append(sf::Vertex{ sf::Vector2f{static_cast<float>(pt.x), wSize.y - static_cast<float>(pt.y)}, col });
            window.draw(curve);
        }
    }
    else // draw control points until the curve satisfies m = n + p + 1 -> draw interpolation points
    {
        auto drawPoints{ [&](std::vector<Point>& points) {
            auto wSize{ window.getSize() };
            for (const auto& p : points)
            {
                sf::CircleShape c{ 5 };
                c.setFillColor(col);
                c.setPosition(static_cast<float>(p.x) - c.getRadius(), wSize.y - static_cast<float>(p.y) - c.getRadius());
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

    sf::VertexArray ch{ sf::LineStrip }; //, convexHull.size() + 1};
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

    if (DEBUG) { logFile << std::format("decompose() u1: {}, u2: {}\n", u1, u2); }

    if (u1 < 0 || u2 < 0)
    {
        if (DEBUG)
        {
            logFile << "decompose() negative knot, return\n";
        }
        return {};
    }

    if (u1 < knotVector.front() || u2 > knotVector.back())
    {
        if (DEBUG)
        {
            logFile << "decompose() interval is outside the knot vector, return\n";
        }
        return {};
    }


    if (std::ssize(knotVector) - 1 != cp_n + p_degree + 1)
    {
        if (DEBUG) { logFile << "decompose(); m = n + p + 1 not satisfied, return\n"; }
        return {};
    }

    if (u1 == knotVector[0] && u2 == knotVector[cp_n + 1])
    {
        if (DEBUG)
        {
            logFile << "decompose() self decomposition, return\n";
        }
        return *this;
    }

    int pre_a, pre_b; // a, b: indices in knotVector
    int m1, m2; // multiplicity for u1 and u2, respectively
    m1 = m2 = p_degree + 1;

    pre_a = findKnotSpan(u1);
    pre_b = findKnotSpan(u2);

    if (u1 == knotVector[pre_a])
    {
        m1 = findKnotMult(pre_a);
        m1 = p_degree + 1 - m1;
    }

    if (u2 == knotVector[cp_n + 1])
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
            logFile << "decompose() knot vector is wrong\n";
        }
        return {};
    }

    ++pre_b;

    int sum{ m1 + m2 };
    if (sum == 0)
    {
        if (DEBUG)
        {
            logFile << "decompose() no knot insertion, return\n";
        }
        return {};
    }
    std::vector<double> uBar;
    std::vector<Point> qw;

    if (DEBUG) { logFile << std::format("decompose() pre_a: {}, pre_b: {}, m1: {}, m2: {}\n", pre_a, pre_b, m1, m2); }


    std::vector<double> X;
    for (int j{}; j < m1; ++j)
        X.push_back(u1);

    for (int j{ m1 }; j < sum; ++j)
        X.push_back(u2);

    // find new indices for X vector
    int post_a{ findKnotSpan(X.front()) };
    int post_b{ findKnotSpan(X.back()) };

    ++post_b;

    if (DEBUG)
    {
        logFile << std::format("decompose() post_a: {}, post_b: {}\n", post_a, post_b);
    }

    int r{ static_cast<int>(std::ssize(X)) - 1 };
    int m{ cp_n + p_degree + 1 };
    uBar.resize(m + r + 2);
    qw.resize(cp_n + r + 2);

    for (int j{}; j <= post_a - p_degree; ++j)
    {
        qw[j] = controlPoints[j];
    }
    for (int j{ post_b - 1 }; j <= cp_n; ++j)
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

        int ind;
        double alpha;
        for (int L{ 1 }; L <= p_degree; ++L) {
            ind = k - p_degree + L;
            alpha = uBar[k + L] - X[j];
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
                        logFile << "decompose() index out of range\n";
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


    int end;

    if (u2 == knotVector[cp_n + 1])
    {
        end = pre_b + m1 + p_degree + 1;
    }
    else
    {
        end = pre_b + m1 + m2;
    }

    int bias{ pre_a + m1 - p_degree };
    std::vector<double> newKnots(end - bias);

    for (size_t j{ 0 }; j < newKnots.size(); ++j)
    {
        newKnots[j] = uBar[j + bias];
        if (newKnots[j] < 0)
        {
            if (DEBUG)
            {
                logFile << "decompose() new knot is negative\n";
            }
            return {};
        }
    }


    Bspline decomposed{ p_degree, newKnots };
    size_t num_control_points{ newKnots.size() - p_degree - 1 };
    for (size_t j{ 0 }; j < num_control_points; ++j)
    {
        decomposed.addPoint(qw[j + bias]);
    }

    if (!decomposed.checkNumbers())
    {
        if (DEBUG)
        {
            logFile << "decompose() decomposed curve error; m = n + p + 1 not satisfied, return\n";
        }
        return {};
    }

    return decomposed;
} //end decompose

void findIntersection(const Bspline& crv1, const Bspline& crv2, std::vector<Point>& iPoints, int& iter, bool lineDetection)
{
    try
    {
        if (!crv1.checkNumbers() || !crv2.checkNumbers())
        {
            std::cout << "m = n + p + 1 not satisfied\n";
            return;
        }

        std::queue<std::pair<Bspline, Bspline>> bqueue;
        bqueue.push(std::pair{ crv1, crv2 });
        bqueue.front().first.interpolationPoints.clear();
        bqueue.front().second.interpolationPoints.clear();
        while (!bqueue.empty() && iter < Bspline::max_iteration && iPoints.size() < Bspline::max_num_intersection_points)
        {
            searchIntersection(bqueue, iPoints, iter, lineDetection);
            bqueue.pop();
        }

        printResult(iter, iPoints, Bspline::DEBUG, Bspline::logFile);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void Bspline::findLineThruEndPoints()
{
    Point pt0{ controlPoints.front() };
    Point ptn{ controlPoints.back() };
    if (pt0.hasSameCoord(ptn))
    {
        if (DEBUG)
        {
            logFile << "findLineThruEndPoints() first control point coincides with the last one\n";
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

    for (int i{ 1 }; i < cp_n; ++i) // the first and the last control point has the same distance
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

    for (int i{ 1 }; i <= cp_n; ++i)
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

    for (int i{}; i <= crv.cp_n; ++i)
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
        std::cout << std::format("maximum number of iteration reached({}), return\n", Bspline::max_iteration);
        if (Bspline::DEBUG) { Bspline::logFile << std::format("maximum number of iteration reached({}), return\n", Bspline::max_iteration); }
        return true;
    }

    if (numIntersectionPoints > Bspline::max_num_intersection_points)
    {
        std::cout << std::format("The number of intersection points reached({}), return\n", Bspline::max_num_intersection_points);
        if (Bspline::DEBUG) { Bspline::logFile << std::format("The number of intersection points reached({}), return\n", Bspline::max_num_intersection_points); }
        return true;
    }

    return false;
}

void searchIntersection(std::queue<std::pair<Bspline, Bspline>>& bqueue, std::vector<Point>& iPoints, int& iter, bool lineDetection)
{
    ++iter;
    Bspline& crv1{ bqueue.front().first };
    Bspline& crv2{ bqueue.front().second };

    if (Bspline::DEBUG)
    {
        Bspline::logFile << std::format("\n~~~ Iteration #{} ~~~\n", iter);
        Bspline::logFile << "queue size: " << bqueue.size() << '\n';
        Bspline::logFile << std::format("curve A u1: {}, u2: {}, deltaU: {}\n", crv1.knotVector.front(), crv1.knotVector.back(), crv1.knotVector.back() - crv1.knotVector.front());
        Bspline::logFile << std::format("curve B u1: {}, u2: {}, deltaU: {}\n", crv2.knotVector.front(), crv2.knotVector.back(), crv2.knotVector.back() - crv2.knotVector.front());
        Bspline::logFile << "curve A:\n";
        crv1.printInfo();
        Bspline::logFile << "curve B:\n";
        crv2.printInfo();
    }

    // check if maximum numbers are reached
    if (exceedsMaximums(iter, iPoints.size()))
        return;

    double deltaU1{ crv1.knotVector[crv1.cp_n + 1] - crv1.knotVector[0] };
    double deltaU2{ crv2.knotVector[crv2.cp_n + 1] - crv2.knotVector[0] };

    if (deltaU1 < 0 || deltaU2 < 0)
    {
        if (Bspline::DEBUG)
        {
            Bspline::logFile << "searchIntersection() negative delta u, return\n";
        }
        return;
    }

    if (deltaU1 < Bspline::u_epsilon && deltaU2 < Bspline::u_epsilon)
    {

        if (Bspline::DEBUG)
        {
            Bspline::logFile << "deltaU1 and deltaU2 are within tolerance: candidate for intersection\n";
        }

        Point intersectPt, intersectPt2;
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
            Bspline::logFile << std::format("u on curve A: {}, u on curve B: {}\n", crv1.knotVector[0] + deltaU1 / 2.0, crv2.knotVector[0] + deltaU2 / 2.0);
            Bspline::logFile << std::format("a candidate point on curve A: ({}, {})\n", intersectPt.x, intersectPt.y);
            Bspline::logFile << std::format("a candidate point on curve B: ({}, {})\n", intersectPt2.x, intersectPt2.y);
        }

        if (std::abs(intersectPt.x - intersectPt2.x) < Point::epsilon && std::abs(intersectPt.y - intersectPt2.y) < Point::epsilon)
        {
            if (Bspline::DEBUG) { Bspline::logFile << "=== Intersection found === \n"; }
            iPoints.push_back(intersectPt);
            return;
        }
        else
        {
            if (Bspline::DEBUG) { Bspline::logFile << "Two points are not within tolerance. no intersection. continuing...\n"; }
        }
    }

    if (deltaU1 > deltaU2)
    {
        if (Bspline::DEBUG) { Bspline::logFile << "switching two curves\n"; }
        bqueue.push(std::pair{ std::move(crv2), std::move(crv1) });
        return;
    }

    if (deltaU1 < Bspline::u1_epsilon)
    {
        if (Bspline::DEBUG) { Bspline::logFile << "one curve becoming a point. continuing ...\n"; }
    }

    if (deltaU1 < Bspline::u2_epsilon && deltaU2 < Bspline::u2_epsilon)
    {
        if (Bspline::DEBUG) { Bspline::logFile << "deltaU1 < u2_epsilon and deltaU2 < u2_epsilon. no more iteration. return\n"; }
        return;
    }

    // line or point detection
    // on: simple line intersection between straight lines, no iterative solution
    // off: try to find intersection, the number of iteration is limited by Bspline::max_iteration or Bspline::max_num_intersection_points whichever comes first
    if (lineDetection && findPointLine(crv1, crv2, iPoints))
        return;

    crv1.checkPoint();
    crv2.checkPoint();

    crv1.findMinMaxDistance();

    if (Bspline::DEBUG) { Bspline::logFile << std::format("min, max of clipping lines of curve A: {}, {}\n", crv1.minDist, crv1.maxDist); }

    if (crv1.maxDist == crv1.minDist)  // (std::abs(maxDist - minDist) < epsilon)
    {
        if (Bspline::DEBUG) { Bspline::logFile << std::format("curve A is a line segment\n"); }
    }

    Bspline distanceCurve{ Bspline(crv2.p_degree, crv2.knotVector) };

    double min{}, max{};

    crv1.makeDistanceCurve(distanceCurve, crv2, min, max);
    if (Bspline::DEBUG) { Bspline::logFile << std::format("minimum and maximum of distance curve: {}, {}\n", min, max); }

    // check whether two line segments on the same line
    if (std::abs(crv1.maxDist - crv1.minDist) < Bspline::u2_epsilon && std::abs(max - min) < Bspline::u2_epsilon && std::abs(crv1.minDist - min) < Bspline::u2_epsilon)
    {
        // recalculate distance from 90 degree rotated line
        crv1.findMinMaxDistanceFromRotatedLine();

        // update distance curve
        crv1.makeDistanceCurve(distanceCurve, crv2, min, max);

        if (Bspline::DEBUG)
        {
            Bspline::logFile << "Two line segments are on the same line rotating clipping lines 90 degrees.\n";
            Bspline::logFile << std::format("min, max of clipping lines of curve A: {}, {}\n", crv1.minDist, crv1.maxDist);
            Bspline::logFile << std::format("minimum and maximum of distance curve: {}, {}\n", min, max);
        }
    }

    if (Bspline::DEBUG)
    {
        Bspline::logFile << "distance curve information:\n";
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
            if (Bspline::DEBUG) { Bspline::logFile << "y value of distance curve adjusted to minDist\n"; }
        }
        else if (cp.y != crv1.maxDist && std::abs(cp.y - crv1.maxDist) < Bspline::epsilon)
        {
            cp.y = crv1.maxDist;
            distanceCurve.isConvexHullUpdated = false;
            adjusted = true;
            if (Bspline::DEBUG) { Bspline::logFile << "y value of distance curve adjusted to maxDist\n"; }
        }
    }

    if (adjusted)
    {
        auto comp = [](const Point& a, const Point& b) -> bool {return a.y < b.y; };
        const auto [small, big] = std::minmax_element(distanceCurve.controlPoints.begin(), distanceCurve.controlPoints.end(), comp);
        min = small->y;
        max = big->y;

        if (Bspline::DEBUG)
        {
            Bspline::logFile << std::format("minimum and maximum of distance curve: {}, {}\n", min, max);
            Bspline::logFile << "distance curve information:\n";
            distanceCurve.printInfo();
        }
    }

    if (min > crv1.maxDist || max < crv1.minDist) // outside the clipping lines: no intersection
    {
        if (Bspline::DEBUG) { Bspline::logFile << "curve B is outside the clipping lines\n"; }
        return;
    }

    distanceCurve.findConvexHull();

    if (distanceCurve.convexHull.size() < 2)
    {
        if (Bspline::DEBUG) { Bspline::logFile << "distanceCurve.controlPoints.size() < 2\n"; }
        throw std::runtime_error("distanceCurve.controlPoints.size() < 2");
    }

    Point pt{ distanceCurve.controlPoints.front() };
    Point pt2{ distanceCurve.controlPoints.back() };

    // at least one of two end points are inside the clipping lines: case 1
    if ((crv1.minDist < pt.y && pt.y < crv1.maxDist) || (crv1.minDist < pt2.y && pt2.y < crv1.maxDist))
    {
        if (Bspline::DEBUG) { Bspline::logFile << "halving curve B\n"; }

        auto bs1{ crv2.decompose(crv2.knotVector[0], crv2.knotVector[0] + deltaU2 / 2.0) };
        if (bs1)
        {
            bqueue.push(std::pair{ std::move(*bs1), crv1 });
            if (Bspline::DEBUG) { Bspline::logFile << "a pair of curves added to bqueue in case 1-1\n"; }
        }

        auto bs2{ crv2.decompose(crv2.knotVector[0] + deltaU2 / 2.0, crv2.knotVector[crv2.cp_n + 1]) };
        if (bs2) {
            bqueue.push(std::pair{ std::move(*bs2), std::move(crv1) });
            if (Bspline::DEBUG) { Bspline::logFile << "a pair of curves added to bqueue in case 1-2\n"; }
        }

        return;
    }

    // two end points are outside the clipping lines

    std::vector<double> uValues;

    auto findUs = [&](double distance)
    {
        if (crv1.isThereLineIntersection(distance, pt.y, pt2.y))
        {
            if (Bspline::DEBUG) { Bspline::logFile << "intersection between clipping line and one edge of convex hull of the distance curve: \n"; }
            if (pt.y == pt2.y)
            {
                if (Bspline::DEBUG)
                {
                    Bspline::logFile << "   the edge is horizontal (distance curve), u values: \n";
                    Bspline::logFile << pt.x << ' ' << pt2.x << '\n';
                }
                uValues.push_back(pt.x);
                uValues.push_back(pt2.x);
            }
            else
            {
                uValues.push_back((pt2.x - pt.x) * (distance - pt.y) / (pt2.y - pt.y) + pt.x);
                if (Bspline::DEBUG) { Bspline::logFile << "   u: " << uValues.back() << '\n'; }
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
        if (Bspline::DEBUG) { Bspline::logFile << "the number of clipped u values < 1, returning\n"; }
        return;
    }

    double u_min{ *std::min_element(uValues.begin(), uValues.end()) };
    double u_max{ *std::max_element(uValues.begin(), uValues.end()) };

    if (std::isnan(u_min) || std::isnan(u_max))
    {
        if (Bspline::DEBUG) { Bspline::logFile << "u value is NaN, returning\n"; }
        return;
    }

    if (std::isinf(u_min) || std::isinf(u_max))
    {
        if (Bspline::DEBUG) { Bspline::logFile << "u value is Inf, returning\n"; }
        return;
    }

    if (u_max == u_min)
    {
        if (Bspline::DEBUG) { Bspline::logFile << "The interval of the u on the clipped curve is zero: a point\n"; }
    }

    if (Bspline::DEBUG) { Bspline::logFile << std::format("u_min: {}, u_max: {}\n", u_min, u_max); }

    if (u_min < crv2.knotVector.front())
    {
        if (u_max == u_min || u_max < crv2.knotVector.front())
        {
            u_max = crv2.knotVector.front();
            if (Bspline::DEBUG) { Bspline::logFile << "u_max adjusted to u_0\n"; }
        }

        u_min = crv2.knotVector.front(); // due to calculation error u_min could be smaller than u_0

        if (Bspline::DEBUG) { Bspline::logFile << "u_min adjusted to u_0\n"; }
    }

    if (u_max > crv2.knotVector.back())
    {
        if (u_min == u_max || u_min > crv2.knotVector.back())
        {
            u_min = crv2.knotVector.back();
            if (Bspline::DEBUG) { Bspline::logFile << "u_min adjusted to u_m\n"; }
        }
        u_max = crv2.knotVector.back(); // due to calculation error u_max could be slightly larger than u_m
        if (Bspline::DEBUG) { Bspline::logFile << "u_max adjusted to u_m\n"; }
    }

    if ((u_max - u_min) > deltaU2 * 0.8) // case 2
    {
        if (Bspline::DEBUG) { Bspline::logFile << "halving curve B\n"; }
        auto bs1{ crv2.decompose(crv2.knotVector[0], crv2.knotVector[0] + deltaU2 / 2.0) };
        if (bs1)
        {
            bqueue.push(std::pair{ std::move(*bs1), crv1 });
            if (Bspline::DEBUG) { Bspline::logFile << "a pair of curves added to bqueue in case 2-1\n"; }
        }

        auto bs2{ crv2.decompose(crv2.knotVector[0] + deltaU2 / 2.0, crv2.knotVector[crv2.cp_n + 1]) };
        if (bs2) {
            bqueue.push(std::pair{ std::move(*bs2), std::move(crv1) });
            if (Bspline::DEBUG) { Bspline::logFile << "a pair of curves added to bqueue in case 2-2\n"; }
        }

        return;
    }
    else // case 3
    {
        if (Bspline::DEBUG) { Bspline::logFile << std::format("decomposing curve B between {}, {}\n", u_min, u_max); }
        auto decomposedC2{ crv2.decompose(u_min, u_max) };
        if (decomposedC2)
        {
            bqueue.push(std::pair{std::move(*decomposedC2), std::move(crv1)});
            if (Bspline::DEBUG) { Bspline::logFile << "a pair of curves added to bqueue in case 3\n"; }
        }

        return;
    }
} //end searchIntersection

bool findPointLine(Bspline& crv1, Bspline& crv2, std::vector<Point>& iPoints)
{
    crv1.findConvexHull();
    crv2.findConvexHull();

    if (crv1.convexHull.size() <= 2 && crv2.convexHull.size() <= 2) // two line segments or points
    {
        if (Bspline::DEBUG) { Bspline::logFile << "point and line detection\n"; }
        crv1.findLineThruEndPoints();
        crv2.findLineThruEndPoints();

        if (crv1.convexHull.size() == 1 && crv2.convexHull.size() == 1) // point vs. point
        {
            if (Bspline::DEBUG) { Bspline::logFile << "curve A and curve B are points\n"; }
            if (crv1.convexHull.front().hasSameCoordWithTolerance(crv2.convexHull.front()))
            {
                iPoints.push_back(crv1.convexHull.front());
                if (Bspline::DEBUG) { Bspline::logFile << "=== intersection found: two points coincide with each other ===\n"; }
                return true;
            }
        }
        else if (crv1.convexHull.size() == 1 && crv2.convexHull.size() == 2) // point vs. line
        {
            if (Bspline::DEBUG) { Bspline::logFile << "curve A : point, curve B : line\n"; }
            if (crv2.isPointOnLineSegment(crv1.convexHull.front()))
            {
                if (Bspline::DEBUG)
                {
                    Bspline::logFile << "=== intersection found between a point and a line ===\n";
                    Bspline::logFile << std::format("intersection point: ({}, {})\n", crv1.convexHull.front().x, crv1.convexHull.front().y);
                }
                iPoints.push_back(crv1.convexHull.front());
                return true;
            }
        }
        else if (crv1.convexHull.size() == 2 && crv2.convexHull.size() == 1) // line vs. point
        {
            if (Bspline::DEBUG) { Bspline::logFile << "curve A : line, curve B : point\n"; }
            if (crv1.isPointOnLineSegment(crv2.convexHull.front()))
            {
                if (Bspline::DEBUG)
                {
                    Bspline::logFile << "=== intersection found between a point and a line ===\n";
                    Bspline::logFile << std::format("intersection point: ({}, {})\n", crv2.convexHull.front().x, crv2.convexHull.front().y);
                }
                iPoints.push_back(crv2.convexHull.front());
                return true;
            }
        }
    }
    return false;
}

void Bspline::checkPoint()
// if a curve is a point then shrink knot vector [midpoint, midpoint + u2_epsilon]
{
    if (convexHull.size() != 1)
        return;

    double u1{ std::midpoint(knotVector[0], knotVector[cp_n + 1]) };
    double u2{ u1 + u2_epsilon };
    auto decomp{ decompose(u1, u2) };
    if (decomp)
    {
        *this = *decomp;
        if (DEBUG) { logFile << "checkPoint(): curve is a point and knot vector shrunk to " << u1 << ", " << u2 << '\n'; }
    }
}

// decompose each curve on knot and find intersection for combination
void bezierIntersection(const Bspline& crv1, const Bspline& crv2, std::vector<Point>& iPoints, int& iter, bool lineDetection)
{
    try
    {
        if (!crv1.checkNumbers() || !crv2.checkNumbers())
        {
            std::cout << "m = n + p + 1 not satisfied\n";
            return;
        }

        std::vector<std::optional<Bspline>> bezierLists[2];

        for (int i{ crv1.p_degree }; i <= crv1.cp_n; ++i)
        {
            bezierLists[0].push_back(crv1.decompose(crv1.knotVector[i], crv1.knotVector[i + 1]));
        }

        for (int i{ crv2.p_degree }; i <= crv2.cp_n; ++i)
        {
            bezierLists[1].push_back(crv2.decompose(crv2.knotVector[i], crv2.knotVector[i + 1]));
        }

        if (Bspline::DEBUG) { Bspline::logFile << "\nthe number of pre-decompositions: " << bezierLists[0].size() + bezierLists[1].size() << '\n'; }

        for (size_t i{}; i < bezierLists[0].size(); ++i) {
            for (size_t j{}; j < bezierLists[1].size(); ++j)
            {
                if (bezierLists[0][i].has_value() && bezierLists[1][j].has_value())
                {
                    std::queue<std::pair<Bspline, Bspline>> bqueue;
                    bqueue.push(std::pair{ *bezierLists[0][i], * bezierLists[1][j] });
                    while (!bqueue.empty() && iter < Bspline::max_iteration && iPoints.size() < Bspline::max_num_intersection_points)
                    {
                        searchIntersection(bqueue, iPoints, iter, lineDetection);
                        bqueue.pop();
                    }
                }
            }
        }

        printResult(iter, iPoints, Bspline::DEBUG, Bspline::logFile);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
} //end bezierIntersection


bool Bspline::isThereLineIntersection(double y, double y1, double y2) const
{
    if ((y1 > y && y2 > y) || (y1 < y && y2 < y))
        return false;
    else
        return true;
}

bool Bspline::isPointOnLineSegment(const Point& pt)
{
    // find the intersection point between the given line and the perpendicular line passing through the point
    /*double x{ coef_b * coef_b * pt.x - coef_a * coef_b * pt.y - coef_a * coef_c };
    double y{ coef_a * coef_a * pt.y - coef_a * coef_b * pt.x - coef_b * coef_c };*/


    //if (DEBUG) { logFile << std::format("calculated point: ({}, {})\n", x, y); }

    auto d{ coef_a * pt.x + coef_b * pt.y + coef_c };

    if (DEBUG) { logFile << "distance from the line: " << d << '\n'; }

    auto min_x{ std::min(controlPoints.front().x, controlPoints.back().x) };
    auto max_x{ std::max(controlPoints.front().x, controlPoints.back().x) };
    auto min_y{ std::min(controlPoints.front().y, controlPoints.back().y) };
    auto max_y{ std::max(controlPoints.front().y, controlPoints.back().y) };

    // check A: the intersection point coincides with the given point
    // B: the point is inside the line segment
    // due to floating point error check again the point is on either end points of the line segment

    //findConvexHull();

    /*if (
        (
            (std::abs(pt.x - x) < Point::epsilon && std::abs(pt.y - y) < Point::epsilon) &&
            (min_x - Point::epsilon) < x && x < (max_x + Point::epsilon) && (min_y - Point::epsilon) < y && y < (max_y + Point::epsilon)
            ) ||
        pt.hasSameCoordWithTolerance(convexHull.front()) ||
        pt.hasSameCoordWithTolerance(convexHull.back())
        )*/
    if (std::abs(d) < epsilon && (min_x - Point::epsilon) < pt.x && pt.x < (max_x + Point::epsilon) && (min_y - Point::epsilon) < pt.y && pt.y < (max_y + Point::epsilon))
    {
        if (DEBUG) { logFile << "a point is on the line\n"; }
        return true;
    }

    if (DEBUG) { logFile << "a point is not on the line\n"; }
    return false;
}

void Bspline::printInfo() // debug only
{
    findConvexHull();

    logFile << "---start curve info----------------------------------------------------------------------\n";
    logFile << "degree: " << p_degree << '\n';
    int i;
    logFile << "knot vector\n";
    for (const auto& x : knotVector)
        logFile << " " << x;

    logFile << '\n';
    logFile << "number of control points: " << (cp_n + 1) << '\n';
    i = 0;
    for (const auto& pt : controlPoints)
    {
        logFile << i++ << '\t' << pt << '\n';
    }

    if (!convexHull.empty()) {
        logFile << "Convex Hull\n";
        for (i = 0; i < convexHull.size(); ++i)
            logFile << "   " << convexHull[i];
    }

    logFile << '\n';
    logFile << "---end curve info------------------------------------------------------------------------\n";
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
    size_t m{ interpolationPoints.size() + p_degree };
    if (m < (p_degree + 1) * 2)
    {
        controlPoints.clear();
        std::cerr << "No. of interpolation points not enough. Add more points\n";
        return;
    }

    // 1. compute u_bar_k
    std::vector<double> u_bar_k;
    find_u_bar_k(u_bar_k);

    // 2. compute U
    find_U(u_bar_k);

    // 3. initialize array A to 0
    std::vector<std::vector<double>> A;
    A.resize(interpolationPoints.size());

    for (auto& x : A)
    {
        x.resize(interpolationPoints.size());
    }

    auto vectorAssign = [&](size_t i, size_t offset) {for (size_t col{ 0 }; col < basis.size(); ++col) { A[i][col + offset] = basis[col]; }};

    for (size_t i{}; i < interpolationPoints.size(); ++i)
    {
        // Set up coefficient matrix
        int span{};
        try
        {
            span = findKnotSpan(u_bar_k[i]);
        }
        catch (const std::exception& e) {
            std::cerr << e.what();
            std::cerr << ": global interpolation failed. Remove the last point.\n";
            return;
        }
        basisFuns(span, u_bar_k[i]);
        vectorAssign(i, span - p_degree);
    }

    std::vector<int> Pm(interpolationPoints.size() + 1); // permutation matrix

    if (!LUPDecompose(A, Pm)) {
        std::cerr << "LUDecomposition failed\n";
        controlPoints.clear();
        if (interpolationPoints.size() >= 2 &&
            interpolationPoints.back().x == interpolationPoints[interpolationPoints.size() - 2].x &&
            interpolationPoints.back().y == interpolationPoints[interpolationPoints.size() - 2].y)
        {
            std::cerr << "Recommendation: remove the last point.\n";
        }
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
        std::cerr << "find_u_bar_k: interpolation points are not enough\n";
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
            std::cerr << std::format("u_bar_k is larger > 1.0: {:.20f}\n", u_bar_k[i]);
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
    std::vector<double> ptr;

    const double Tol{ 1e-6 };

    for (int i{}; i <= N; ++i)
    {
        Pm[i] = i;
    } //Unit permutation matrix, P[N] initialized with N

    for (size_t i{}; i < N; ++i) {
        double maxA{};
        size_t imax{ i };

        double absA{};
        for (size_t k{ i }; k < N; ++k)
            if ((absA = std::abs(A[k][i])) > maxA) {
                maxA = absA;
                imax = k;
            }

        if (maxA < Tol) { return false; } //failure, matrix is degenerate

        if (imax != i) {
            //pivoting P
            int temp{ Pm[i] };
            Pm[i] = Pm[imax];
            Pm[imax] = temp;

            //pivoting rows of A
            ptr = A[i];
            A[i] = A[imax];
            A[imax] = ptr;

            //counting pivots starting from N (for determinant)
            ++Pm[N];
        }

        for (size_t j{ i + 1 }; j < N; ++j) {
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

    size_t N{ A.size() };
    controlPoints.resize(N);

    for (size_t i{}; i < N; i++) {
        controlPoints[i].y = interpolationPoints[Pm[i]].y;
        controlPoints[i].x = interpolationPoints[Pm[i]].x;

        for (size_t k{}; k < i; k++)
        {
            controlPoints[i].x -= A[i][k] * controlPoints[k].x;
            controlPoints[i].y -= A[i][k] * controlPoints[k].y;
        }
    }

    for (size_t i{ N - 1 }; i != -1; --i) {
        for (size_t k{ i + 1 }; k < N; ++k)
        {
            controlPoints[i].x -= A[i][k] * controlPoints[k].x;
            controlPoints[i].y -= A[i][k] * controlPoints[k].y;
        }

        controlPoints[i].x /= A[i][i];
        controlPoints[i].y /= A[i][i];
    }
}