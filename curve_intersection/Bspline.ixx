module;

#include <vector>
#include <optional>
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
	void addPoint(const Point& p);
	void addPointAndKnots(const Point& p);
	void deleteLastPoint();
	void deleteLastPointAndKnots();
	Point getPoint(int i);
	void findConvexHull();
	bool leftTurn(const Point& p0, const Point& p1, const Point& p2);
	bool hasEnoughPoints();
	double getMinPara();
	double getMaxPara();
	void drawControlPolygon(sf::RenderWindow& window);
	//void drawCurveSegments(Graphics g, int h);
	void drawCurve(sf::RenderWindow& window, sf::Color col);
	void drawConvexHull(sf::RenderWindow& window, sf::Color col);
	std::optional<Bspline> decompose(double u1, double u2);
	void findIntersection(Bspline crv, std::vector<Point>& iPoints, std::vector<Bspline>& splines, int& dNum);
	void bezierIntersection(Bspline bs, std::vector<Point>& iPoints, std::vector<Bspline>&, std::vector<Bspline>& splines, int& dNum);
	void printInfo();
	int findKnotSpan(double u);
private:
	int findFirstPointOfConvexHull();

	int findKnotMult(int i);
	void makeKnots();
	void findLineThruEndPoints();
	void findMinMaxDistance();
	bool isThereLineIntersection(double y, double y1, double y2);

	static const double epsilon;
	static const double intersection_epsilon;

	int p_degree{}; // degree
	int cp_n{ -1 }; // n + 1: # of control pts
	std::vector<Point> controlPoints;
	std::vector<double> knotVector;
	std::vector<double> basis, left, right;
	std::vector<Point> convexHull;
	double coef_a{}, coef_b{}, coef_c{}; // ax+by+c=0 line through the end points (normalized form: a^2 + b^2 = 1)
	double minDist{}, maxDist{}; // minimum and maximum distance of control points from the line ax+by+c=0
};
