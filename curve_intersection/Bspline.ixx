// September 2, 2022
// by SM

module;

#include <SFML/Graphics.hpp>

export module Bspline;

import <optional>;
import <vector>;
import <fstream>;
import <string_view>;
import <utility>;
import <queue>;
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
    friend void findIntersection(const Bspline& crv1, const Bspline& crv2, std::vector<Point>& iPoints, int& iter, bool lineDetection); // check curves first and proceed, call searchIntersection()
    friend void bezierIntersection(const Bspline& crv1, const Bspline& crv2, std::vector<Point>& iPoints, int& iter, bool lineDetection);
    void printInfo();
    void globalCurveInterpolation();
    int findKnotSpan(double u) const;
    bool checkNumbers() const; // check m = n + p + 1
    void clear();
    void clearControlPoints();
    void changeDegree(int degree);
    void find_u_bar_k(std::vector<double>& u_bar_k);
    void find_U(const std::vector<double>& u_bar_k);
    static bool DEBUG;
    static bool controlPointMode;
    friend void loadPoints(Bspline& curve1, Bspline& curve2, std::string_view filePathName);
    friend void savePoints(const Bspline& curve1, const Bspline& curve2, std::string_view filePathName);
    void LUPSolve(const std::vector<std::vector<double>>& A, const std::vector<int>& Pm);
    bool LUPDecompose(std::vector<std::vector<double>>& A, std::vector<int>& Pm);
    static std::ofstream logFile;
private:
    void deleteLastPoint();
    friend void searchIntersection(std::queue<std::pair<Bspline, Bspline>>& bqueue, std::vector<Point>& iPoints, int& iter, bool lineDetection); // internal call
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
    double coef_a{}, coef_b{}, coef_c{}; // ax + by + c = 0 line through the end points (normalized form: a^2 + b^2 = 1)
    double minDist{}, maxDist{}; // minimum and maximum distance of control points from the line, ax + by + c = 0
    bool isConvexHullUpdated{ false }; // prevent the same calculations once convex hull is updated
};
