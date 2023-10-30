// September 2, 2022
// by SM

module;

#include "pch.h"

export module Bspline;

import Point;
import NodeInfo;

struct TwoCurves;
struct ParamInfo;

export class Bspline
{
public:
    explicit Bspline(int degree) : p_degree{ degree }
    {
        basis.resize(p_degree + 1); left.resize(p_degree + 1); right.resize(p_degree + 1);
    }
    Bspline(int degree, const std::vector<double>& knotVec) : Bspline{ degree } { knotVector = knotVec; }
    void basisFuns(int i, double u);
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
    friend void predecomposeFindIntersection(Bspline crv1, Bspline crv2, std::queue<TwoCurves>& bQueue);
    friend void loadPoints(Bspline& curve1, Bspline& curve2, const std::string& filePathName);
    friend void savePoints(const Bspline& curve1, const Bspline& curve2, const std::string& filePathName);
private:
    void deleteLastPoint();
    friend void searchIntersection(std::queue<TwoCurves>& bQueue, std::vector<Point>& iPoints, int& iter, bool lineDetection, std::vector<NodeInfo>& vNodeInfo);
    friend bool exceedsMaximums(int iter, size_t numIntersectionPoints);
    friend bool findPointLine(Bspline& crv1, Bspline& crv2, std::vector<Point>& iPoints);
    void addPoint(const Point& p);
    int findFirstPointOfConvexHull() const;
    bool isPointOnLineSegment(const Point& pt);
    int findKnotMult(int i) const;
    void makeKnots();
    void findLineThruEndPoints();
    void findMinMaxDistance(); // from line through cp_0 and cp_n
    void findMinMaxDistanceFromRotatedLine(); // from 90 degree rotated line
    void makeDistanceCurve(Bspline& distanceCurve, const Bspline& crv, double& min, double& max);
    bool isThereLineIntersection(double y, double y1, double y2) const;
    bool leftTurn(const Point& p0, const Point& p1, const Point& p2) const;
    void clearControlPoints();
    int findKnotSpan(double u) const;
    void find_u_bar_k(std::vector<double>& u_bar_k);
    void find_U(const std::vector<double>& u_bar_k);
    void LUPSolve(const std::vector<std::vector<double>>& A, const std::vector<int>& Pm);
    bool LUPDecompose(std::vector<std::vector<double>>& A, std::vector<int>& Pm);
    void printInfo();
    void checkPointToShrink();

    static const double epsilon;
    static const double u_epsilon;
    static const double u1_epsilon;
    static const double u2_epsilon;
    static const int max_iteration;
    static const int max_num_intersection_points;

    int p_degree{}; // degree
    int cp_n{ -1 }; // cp_n + 1: # of control pts
    std::vector<Point> controlPoints;
    std::vector<Point> interpolationPoints;
    std::vector<double> knotVector;
    std::vector<double> basis, left, right;
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
    double ua;
    double ub;
    Point iPoint;
};

export struct ParamInfo
{
    bool lineDetection;
    bool decomposeFirst;
    int iterationNum;
    std::vector<Point> iPoints;
    std::vector<NodeInfo> vNodeInfo;
};