// September 2, 2022
// by SM

module;

#include <optional>
#include <vector>
#include <string_view>
#include <SFML/Graphics.hpp>

export module Bspline;

import Point;

export class Bspline {
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
    Point getPoint(int i) const;
    void findConvexHull();
    bool leftTurn(const Point& p0, const Point& p1, const Point& p2) const;
    bool hasEnoughPoints() const;
    void drawControlPolygon(sf::RenderWindow& window) const;
    void drawCurve(sf::RenderWindow& window, sf::Color col);
    void drawConvexHull(sf::RenderWindow& window, sf::Color col);
    std::optional<Bspline> decompose(double u1, double u2) const;
    void findIntersection(Bspline crv, std::vector<Point>& iPoints, int& iter, bool lineDetection); // check curves first and proceed, call searchIntersection()
    void bezierIntersection(Bspline bs, std::vector<Point>& iPoints, int& iter, bool lineDetection);
    void printInfo();
    void globalCurveInterpolation();
    int findKnotSpan(double u) const;
    bool checkNumbers() const; // check m = n + p + 1
    void clear();
    void chageDegree(int degree);
    void find_u_bar_k(std::vector<double>& uk);
    void find_U(const std::vector<double>& uk, size_t n);
    static bool DEBUG;
    friend void loadPoints(Bspline& curve1, Bspline& curve2, std::string_view filePathName);
    friend void savePoints(const Bspline& curve1, const Bspline& curve2, std::string_view filePathName);
    void LUPSolve(const std::vector<std::vector<double>>& A, const std::vector<int>& P);
    bool LUPDecompose(std::vector<std::vector<double>>& A, size_t N, std::vector<int>& P);
private:
    void deleteLastPoint();
    void searchIntersection(Bspline crv, std::vector<Point>& iPoints, int& iter, bool lineDetection); // internal call
    void addPoint(const Point& p);
    int findFirstPointOfConvexHull() const;
    bool isPointOnLineSegment(const Point& pt) const;
    int findKnotMult(int i) const;
    void makeKnots();
    void findLineThruEndPoints();
    void findMinMaxDistance(); // from line through cp_0 and cp_n
    bool isThereLineIntersection(double y, double y1, double y2) const;

    static const double epsilon;
    static const double u_epsilon;
    static const double u1_epsilon;
    static const int max_iteration;
    static const int max_num_intersection_points;

    int p_degree{}; // degree
    int cp_n{ -1 }; // n + 1: # of control pts
    std::vector<Point> controlPoints;
    std::vector<Point> interpolationPoints;
    std::vector<double> knotVector;
    std::vector<double> basis, left, right;
    std::vector<Point> convexHull;
    double coef_a{}, coef_b{}, coef_c{}; // ax + by + c = 0 line through the end points (normalized form: a^2 + b^2 = 1)
    double minDist{}, maxDist{}; // minimum and maximum distance of control points from the line ax + by + c = 0
    bool isConvexHullUpdated{ false }; // prevent the same calculations once convex hull is updated
};
