// September 2, 2022
// by SM

module;

//#include "pch.h"
#include <SFML/Graphics.hpp>

export module Bspline;

import NodeInfo;
import Point;
import std;

struct TwoCurves;
struct ParamInfo;

export class Bspline
{
public:
  explicit Bspline(int degree) : p_degree{ degree }
  {
    if (degree < 1 || degree > max_degree)
    {
      throw std::invalid_argument{ "the degree of the curve is out of range" };
    }
  }
  Bspline(int degree, const std::vector<double>& knotVec) : Bspline{ degree } { knotVector = knotVec; }
  void curvePoint(double u, Point& pt);
  void addPointAndKnots(const Point& p);
  void addInterpolationPoint(const Point& p);
  void deleteLastPointAndKnots();
  void deleteLastInterpolationPoint();
  size_t getControlPointSize() const { return controlPoints.size(); }
  size_t getInterpolationPointSize() const { return interpolationPoints.size(); }
  void findConvexHull();
  bool hasEnoughPoints() const;
  void drawControlPolygon(sf::RenderWindow& window) const;
  void drawCurve(sf::RenderWindow& window, sf::Color col, sf::VertexArray& va);
  void drawConvexHull(sf::RenderWindow& window, sf::Color col);
  std::optional<Bspline> decompose(double u1, double u2) const;
  void globalCurveInterpolation();
  bool checkNumbers() const; // check m = n + p + 1
  void clear();
  void changeDegree(int degree);
  int getDegree() const { return p_degree; }
  int getID() const { return id; }
  void setID(int i) { id = i; }

  static bool DEBUG;
  static std::ofstream logFile;

  bool interpolationMode{ true }; // true: interpolation, false: control points

  friend void findIntersection(Bspline crv1, Bspline crv2, ParamInfo& paramInfo);
  friend void predecomposeCurves(Bspline crv1, Bspline crv2, std::queue<TwoCurves>& bQueue);
  friend void loadPoints(Bspline& curve1, Bspline& curve2, const std::string& filePathName);
  friend void savePoints(const Bspline& curve1, const Bspline& curve2, const std::string& filePathName);
private:
  void deleteLastPoint();
  friend void searchIntersection(std::queue<TwoCurves>& bQueue, ParamInfo& paramInfo);
  friend bool exceedsMaximums(int iter, size_t numIntersectionPoints);
  void addPoint(const Point& p);
  int findFirstPointOfConvexHull() const;
  int findKnotMult(int i) const;
  void makeKnots();
  void findLineThruEndPoints();
  void findMinMaxDistance(); // from line through cp_0 and cp_n
  void findMinMaxDistanceFromRotatedLine(); // from 90 degree rotated line
  void makeDistanceCurve(Bspline& distanceCurve, const Bspline& crv, double& min, double& max);
  bool isThereLineIntersection(double y, double y1, double y2) const;
  bool leftTurn(const Point& p0, const Point& p1, const Point& p2) const;
  void clearControlPoints();
  int findKnotSpan(double u, int control_point_n) const;
  void find_u_bar_k(std::vector<double>& u_bar_k);
  void find_U(const std::vector<double>& u_bar_k);
  void LUPSolve(const std::vector<std::vector<double>>& A, const std::vector<int>& Pm);
  bool LUPDecompose(std::vector<std::vector<double>>& A, std::vector<int>& Pm);
  void printInfo();
  void checkPointToShrink();

  // cp_n: the last index of control points
  // cp_n + 1: # of control points
  int cp_n() const { return static_cast<int>(std::ssize(controlPoints)) - 1; }

  static void basisFuns(const Bspline& crv, int i, double u);
  static constexpr double epsilon{ 1e-9 }; // epsilon is for approximate zero and should be much less than u_epsilon
  static constexpr double rotation_epsilon{ 0.001 }; // to rotate clipping lines
  static constexpr double u_epsilon{ 0.0001 }; // for knot values
  static constexpr double u1_epsilon{ u_epsilon / 10.0 };
  static constexpr double u2_epsilon{ u1_epsilon / 10.0 };
  static constexpr int max_iteration{ 500'000 }; // maximum iteration for overlapping curves
  static constexpr int max_num_intersection_points{ 500'000 };
  static constexpr int max_degree{ 10 }; // maximum degree of a curve

  // for basisFuns function
  static std::array<double, max_degree + 1> basis;

  int p_degree{ 1 }; // degree
  std::vector<Point> controlPoints;
  std::vector<Point> interpolationPoints;
  std::vector<double> knotVector;
  std::vector<Point> convexHull;

  // ax + by + c = 0 line through the end points (normalized form: a^2 + b^2 = 1)
  double coef_a{};
  double coef_b{};
  double coef_c{};

  // minimum and maximum distance of control points from the line, ax + by + c = 0
  double minDist{};
  double maxDist{};

  bool isConvexHullUpdated{}; // prevent the same calculations once convex hull is updated
  bool drawUpdated{};
  int id{ -1 };
};

export struct TwoCurves
{
  Bspline c1;
  Bspline c2;
  int parentIter;
  int depth;
};

export struct IPointInfo
{
  CurveInfo ci;
  Point p;
};

export struct ParamInfo
{
  bool decomposeFirst;
  int iterationNum;
  std::vector<Point> iPoints;
  std::vector<NodeInfo> vNodeInfo;
};
