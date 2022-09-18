// September 2, 2022
// by SM
//
// numbered algorithms are from 'The NURBS Book' by Les Piegl and Wayne Tiller, publisher: Springer, ISBN 3-540-55069-0

module;

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <chrono>
#include <algorithm>
#include <format>
#include <stdexcept>
#include <SFML/Graphics.hpp>

module Bspline;

//import <iostream>;
//import <string>;
//import <vector>;
//import <list>;
//import <chrono>;
//import <algorithm>;
//import <format>;
//import <stdexcept>;

bool Bspline::debug{ false };
const double Bspline::epsilon{ 1e-9 }; // epsilon is for approximate zero and should be much less than u_epsilon
const double Bspline::u_epsilon{ 0.0001 }; // for knot values
const double Bspline::u1_epsilon{ u_epsilon / 10.0 }; // for delta u1
const int Bspline::max_iteration{ 20'000 }; // maximum iteration for overlapping curves
const int Bspline::max_num_intersection_points{ 1'000 };

int Bspline::findKnotSpan(double u) const
{
	if (u < knotVector.front() || u > knotVector.back())
	{
		if (debug) { std::cout << "findKnotSpan() u is outside of range\n"; }
		throw std::runtime_error("findKnotSpan() u is outside of range");
	}

	//algorithm A2.1 FindSpan pp68
	if (u == knotVector[cp_n + 1]) // special case // ? (std::abs(u - knotVector[cp_n + 1]) < epsilon) 
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
	knotVector.clear();
	convexHull.clear();
	isConvexHullUpdated = false;
	cp_n = -1;
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

void Bspline::addPoint(const Point& p)
{
	controlPoints.push_back(p);
	cp_n = static_cast<int>(controlPoints.size()) - 1;
	isConvexHullUpdated = false;
}

void Bspline::addPointAndKnots(const Point& p)
{
	addPoint(p);
	makeKnots();
}

void Bspline::deleteLastPoint()
{
	controlPoints.pop_back();
	cp_n = static_cast<int>(controlPoints.size()) - 1;
	isConvexHullUpdated = false;
}

void Bspline::deleteLastPointAndKnots()
{
	if (controlPoints.size() == 0)
		return;

	deleteLastPoint();
	makeKnots();
}

Point Bspline::getPoint(int i) const
{
	return controlPoints[i];
}

int Bspline::findFirstPointOfConvexHull() const
{
	size_t pos{ 0 };

	for (size_t i{ 1 }; i < controlPoints.size(); ++i)
	{
		if (controlPoints[i].y < controlPoints[pos].y)
		{
			pos = i;
		}
		else if (controlPoints[i].y == controlPoints[pos].y &&
			controlPoints[i].x < controlPoints[pos].x)
		{
			if (debug)
			{
				std::cout << "findFirstPointOfConvexHull() two points have the same y values\n";
			}
			pos = i;
		}
	}
	return static_cast<int>(pos);
}

void Bspline::findConvexHull()
{
	if (isConvexHullUpdated || controlPoints.size() == 0)
	{
		return;
	}

	if (controlPoints.size() == 1) // ? needs more thoughts ...
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
		if (i != firstCHPoint)
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
				if (min->findDistance(controlPoints[firstCHPoint]) >= index->findDistance(controlPoints[firstCHPoint])) {
					auto temp{ std::prev(index) };
					unsortedPoints.erase(index);
					index = temp;
				}
				else {
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

	if (debug) { std::cout << std::format("decompose() u1: {}, u2: {}\n", u1, u2); }

	if (u1 < 0 || u2 < 0)
	{
		if (debug)
		{
			std::cout << "decompose() negative knot, return\n";
		}
		return {};
	}

	if (u1 < knotVector.front() || u2 > knotVector.back())
	{
		if (debug)
		{
			std::cout << "decompose() interval is outside the knot vector, return\n";
		}
		return {};
	}


	if (static_cast<int>(knotVector.size()) - 1 != cp_n + p_degree + 1)
	{
		if (debug) { std::cout << "decompose(); m = n + p + 1 not satisfied, return\n"; }
		return {};
	}

	if (u1 == knotVector[0] && u2 == knotVector[cp_n + 1])
	{
		if (debug)
		{
			std::cout << "decompose() self decomposition, return\n";
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
		if (debug)
		{
			std::cout << "decompose() knot vector is wrong\n";
		}
		return {};
	}

	++pre_b;

	int sum{ m1 + m2 };
	if (sum == 0)
	{
		if (debug)
		{
			std::cout << "decompose() no knot insertion, return\n";
		}
		return {};
	}
	std::vector<double> uBar;
	std::vector<Point> qw;

	if (debug) { std::cout << std::format("decompose() pre_a: {}, pre_b: {}, m1: {}, m2: {}\n", pre_a, pre_b, m1, m2); }


	std::vector<double> X;
	for (int j{}; j < m1; ++j)
		X.push_back(u1);

	for (int j{ m1 }; j < sum; ++j)
		X.push_back(u2);

	// find new indices for X vector
	int post_a{ findKnotSpan(X.front()) };
	int post_b{ findKnotSpan(X.back()) };

	++post_b;

	if (debug)
	{
		std::cout << std::format("decompose() post_a: {}, post_b: {}\n", post_a, post_b);
	}

	int r{ static_cast<int>(X.size()) - 1 };
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
				if (ind >= static_cast<int>(qw.size()))
				{
					if (debug)
					{
						std::cout << "decompose() index out of range\n";
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
			if (debug)
			{
				std::cout << "decompose() new knot is negative\n";
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
		if (debug)
		{
			std::cout << "decompose() decomposed curve error; m = n + p + 1 not satisfied, return\n";
		}
		return {};
	}

	return decomposed;
} //end decompose

void Bspline::findIntersection(Bspline crv, std::vector<Point>& iPoints, int& iter, bool lineDetection)
{
	if (checkNumbers() && crv.checkNumbers())
	{
		searchIntersection(crv, iPoints, iter, lineDetection);

		std::cout << '\t' << iter << " decomposition(s)\n";

		if (iPoints.size() == 0)
		{
			std::cout << "No intersection\n";
		}
		else
		{
			for (int i{}; i < iPoints.size(); ++i)
			{
				std::cout << std::format("***intersection point #{}: ", i + 1);
				std::cout << iPoints[i] << '\n';
			}
		}
	}
	else
		std::cout << "m = n + p + 1 not satisfied\n";
}

void Bspline::findLineThruEndPoints()
{
	Point pt0{ controlPoints.front() };
	Point ptn{ controlPoints.back() };
	if (pt0.hasSameCoord(ptn))
	{
		if (debug)
		{
			std::cout << "findLineThruEndPoints() first control point coincides with the last one\n";
		}
		coef_a = 0;
		coef_b = 1;
		coef_c = -pt0.y;
	}
	else
	{
		coef_a = ptn.y - pt0.y;
		coef_b = pt0.x - ptn.x;
		double norm = std::hypot(coef_a, coef_b);
		coef_a /= norm;
		coef_b /= norm;
		coef_c = (ptn.x * pt0.y - pt0.x * ptn.y) / norm;
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

void Bspline::searchIntersection(Bspline crv, std::vector<Point>& iPoints, int& dNum, bool lineDetection)
{
	if (debug) 
	{
		std::cout << std::format("~~~ Iteration #{} ~~~\n", dNum);
		std::cout << std::format("curve A u1: {}, u2: {}\n", knotVector.front(), knotVector.back());
		std::cout << std::format("curve B u1: {}, u2: {}\n", crv.knotVector.front(), crv.knotVector.back());
		std::cout << "curve A:\n";
		printInfo();
		std::cout << "curve B:\n";
		crv.printInfo();
	}

	if (dNum > max_iteration)
	{
		std::cout << std::format("maximum number of decomposition reached({}), return\n", max_iteration);
		return;
	}

	if (iPoints.size() > max_num_intersection_points)
	{
		std::cout << std::format("The number of intersection points reached({}), return\n", max_num_intersection_points);
		return;
	}

	double deltaU1{ knotVector[cp_n + 1] - knotVector[0] };
	double deltaU2{ crv.knotVector[crv.cp_n + 1] - crv.knotVector[0] };

	if (deltaU1 < 0 || deltaU2 < 0)
	{
		if (debug)
		{
			std::cout << "searchIntersection() negative delta u, return\n";
		}
		return;
	}

	if (deltaU1 < u_epsilon && deltaU2 < u_epsilon)
	{

		if (debug)
		{
			std::cout << "deltaU1 and deltaU2 are within tolerance: candidate for intersection\n";
		}

		Point intersectPt, intersectPt2;
		if (deltaU1 == 0)
		{
			intersectPt = controlPoints.front();
		}
		else
		{
			curvePoint(knotVector[0] + deltaU1 / 2.0, intersectPt);
		}

		if (deltaU2 == 0)
		{
			intersectPt2 = crv.controlPoints.front();
		}
		else
		{
			crv.curvePoint(crv.knotVector[0] + deltaU2 / 2.0, intersectPt2);
		}

		if (debug)
		{
			std::cout << std::format("u on curve A: {}, u on curve B: {}\n", knotVector[0] + deltaU1 / 2.0, crv.knotVector[0] + deltaU2 / 2.0);
			std::cout << std::format("a candidate point on curve A: ({}, {})\n", intersectPt.x, intersectPt.y);
			std::cout << std::format("a candidate point on curve B: ({}, {})\n", intersectPt2.x, intersectPt2.y);
		}

		if (std::abs(intersectPt.x - intersectPt2.x) < Point::epsilon && std::abs(intersectPt.y - intersectPt2.y) < Point::epsilon)
		{
			if (debug) { std::cout << "=== Intersection found === \n"; }
			iPoints.push_back(intersectPt);
		}
		else
		{
			if (debug) { std::cout << "Two points are not within tolerance. no intersection\n"; }
		}
		return;
	}

	if (deltaU1 > deltaU2)
	{
		if (debug) { std::cout << "switching two curves\n"; }
		crv.searchIntersection(*this, iPoints, dNum, lineDetection);
		return;
	}

	if (deltaU1 < u1_epsilon) // (deltaU1 < u_epsilon) // prevent only one curve is getting smaller, reduce further iterations
	{
		if (debug) { std::cout << "one curve becoming a point.\n"; }

		Point pt1, pt2, pt3;
		if (deltaU1 == 0) // u1 == u2
		{
			pt1 = controlPoints.front();
		}
		else
		{
			curvePoint(knotVector[0] + deltaU1 / 2.0, pt1);
		}

		crv.curvePoint(crv.knotVector.front(), pt2);
		crv.curvePoint(crv.knotVector.back(), pt3);

		if (debug)
		{
			std::cout << std::format("point on curve A: ({}, {})\n", pt1.x, pt1.y);
			std::cout << std::format("start point on curve B: ({}, {})\n", pt2.x, pt2.y);
			std::cout << std::format("end point on curve B : ({}, {})\n", pt3.x, pt3.y);
		}

		// check whether converging point is on the control points of the other curve
		crv.findConvexHull();
		for (auto& x : crv.convexHull) // or crv.controlPoints
		{
			if (pt1.hasSameCoordWithTolerance(x))
			{
				if (debug) { std::cout << "=== intersection found at control points ===\n"; }
				iPoints.push_back(pt1);
				if (debug) { std::cout << "returning\n"; }
				return;
			}
		}

		if (crv.convexHull.size() == 2 && isPointOnLineSegment(pt1, crv))
		{
			if (debug) { std::cout << "=== intersection found between a point and a line segment ===\n"; }
			iPoints.push_back(pt1);
			if (debug) { std::cout << "returning\n"; }
			return;
		}

		auto [x_min, x_max] { std::minmax_element(crv.convexHull.begin(), crv.convexHull.end(), [](const Point& lhs, const Point& rhs) { return lhs.x < rhs.x; }) };
		auto [y_min, y_max] { std::minmax_element(crv.convexHull.begin(), crv.convexHull.end(), [](const Point& lhs, const Point& rhs) { return lhs.y < rhs.y; }) };

		if (pt1.x < x_min->x || pt1.x > x_max->x || pt1.y < y_min->y || pt1.y > y_max->y)
		{
			if (debug) { std::cout << "a point is outside the box containing other's convex hull\n"; }
			return;
		}
		
		if (deltaU1 < epsilon) // prevent infinite loop(stackoverflow)
		{
			if (debug) { std::cout << "last resort, deltaU1 is too small. stop continuing. return\n"; }
			return;
		}

		if (debug) { std::cout << "continuing ...\n"; }
	} // deltaU1 < u_epsilon

	// line detection
	// on: simple line intersection between straight lines, no recursive solution
	// off: try to find intersection, the number of iteration is limited by Bspline::max_iteration or Bspline::max_num_intersection_points whichever comes first
	if (lineDetection)
	{
		findConvexHull();
		crv.findConvexHull();

		if (convexHull.size() == 2 && crv.convexHull.size() == 2) // two line segments or points
		{
			findLineThruEndPoints();
			crv.findLineThruEndPoints();
			double d{ coef_a * crv.coef_b - crv.coef_a * coef_b };

			// check whether they are points
			if (convexHull.front().hasSameCoord(convexHull.back()))
			{
				std::cout << "curve A is a point\n";
				printInfo();
				if (crv.convexHull.front().hasSameCoord(crv.convexHull.back())) // point vs. point
				{
					std::cout << "curve B is a point\n";
					if (convexHull.front().hasSameCoordWithTolerance(crv.convexHull.front()))
					{
						iPoints.push_back(convexHull.front());
						std::cout << "=== intersection found: two points coincide with each other ===\n";
					}
					else
					{
						std::cout << "two points do not coincide with each other\n";
					}
					return;
				}
				else // point vs. line
				{
					if (isPointOnLineSegment(convexHull.front(), crv))
					{
						std::cout << "=== intersection found between a point and a line ===\n";
						std::cout << std::format("intersection point: ({}, {})\n", convexHull.front().x, convexHull.front().y);
						iPoints.push_back(convexHull.front());
					}
					else
					{
						std::cout << "a point is outside the line\n";
					}
					return;
				}
			}
			else if (crv.convexHull.front().hasSameCoord(crv.convexHull.back())) // line vs. point
			{
				std::cout << "curve B is a point\n";
				if (isPointOnLineSegment(crv.convexHull.front(), *this))
				{
					std::cout << "=== intersection found between a point and a line ===\n";
					std::cout << std::format("intersection point: ({}, {})\n", crv.convexHull.front().x, crv.convexHull.front().y);
					iPoints.push_back(crv.convexHull.front());
				}
				else
				{
					std::cout << "no intersection between a point and a line\n";
				}
				return;
			}

			// line vs line
			if (d == 0.0)
			{
				if (coef_c == crv.coef_c)
				{
					if (isPointOnLineSegment(convexHull.front(), crv) || isPointOnLineSegment(convexHull.back(), crv) ||
						isPointOnLineSegment(crv.convexHull.front(), *this) || isPointOnLineSegment(crv.convexHull.back(), *this)
						)
					{
						std::cout << "searchIntersection() *** two line segments are touching or (partially) overlapping ***\n";
					}
					else
					{
						std::cout << "searchIntersection() two line segments are on the same line(not overlapping)\n";
					}
				}
				else
				{
					std::cout << "searchIntersection(); two line segments are parallel\n";
				}

			}
			else
			{
				Point ip{ (coef_b * crv.coef_c - crv.coef_b * coef_c) / d, (crv.coef_a * coef_c - coef_a * crv.coef_c) / d };

				if (isPointOnLineSegment(ip, *this) && isPointOnLineSegment(ip, crv))
				{
					std::cout << "=== intersection found: two line segments ===\n";
					std::cout << std::format("intersection point: ({}, {})\n", ip.x, ip.y);
					iPoints.push_back(ip);
				}
				else
				{
					std::cout << "no intersection between two line segments\n";
				}
			}
			return;
		}
	}// end of two line segments

	findMinMaxDistance();

	if (debug) { std::cout << std::format("min, max of clipping lines of curve A: {}, {}\n", minDist, maxDist); }

	if (maxDist == minDist)  // (std::abs(maxDist - minDist) < epsilon)
	{
		if (debug) { std::cout << std::format("curve A is a line segment\n"); }
	}

	Bspline distanceCurve{ Bspline(crv.p_degree, crv.knotVector) };
	Point pt{ crv.controlPoints.front() };
	double min, max;
	double xi{};
	double d{ coef_a * pt.x + coef_b * pt.y + coef_c };
	min = max = d;

	for (int i{ 0 }; i <= crv.cp_n; ++i)
	{
		xi = 0.0;
		for (int j{ 1 }; j <= crv.p_degree; ++j)
			xi += crv.knotVector[i + j];

		xi /= crv.p_degree;
		pt = crv.controlPoints[i];
		d = coef_a * pt.x + coef_b * pt.y + coef_c;
		distanceCurve.addPoint(Point{ xi, d });
		min = std::fmin(min, d);
		max = std::fmax(max, d);
	}

	if (debug) { std::cout << std::format("minimum and maximum of distance curve: {}, {}\n", min, max); }

	if (min > maxDist || max < minDist) // outside the clipping lines: no intersection
	{
		if (debug) { std::cout << "curve B is outside the clipping lines\n"; }
		return;
	}

	distanceCurve.findConvexHull();

	if (debug)
	{
		std::cout << "distance curve information:\n";
		distanceCurve.printInfo();
	}

	if (distanceCurve.convexHull.size() < 2)
	{
		if (debug) { std::cout << "distanceCurve.controlPoints.size() < 2\n"; }
		throw std::runtime_error("distanceCurve.controlPoints.size() < 2");
	}

	pt = distanceCurve.controlPoints.front();
	Point pt2{ distanceCurve.controlPoints.back() };

	// at least one of two end points are inside the clipping lines
	if ((minDist < pt.y && pt.y < maxDist) || (minDist < pt2.y && pt2.y < maxDist))  // on boundary decompose curve B. cf) if ((minDist <= pt.y && pt.y <= maxDist) || (minDist <= pt2.y && pt2.y <= maxDist)) on boundary decompose curve A
	{
		auto bs1{ decompose(knotVector[0], knotVector[0] + deltaU1 / 2.0) };
		if (bs1)
		{
			++dNum;
			bs1->searchIntersection(crv, iPoints, dNum, lineDetection);
		}
		auto bs2{ decompose(knotVector[0] + deltaU1 / 2.0, knotVector[cp_n + 1]) };
		if (bs2)
		{
			++dNum;
			bs2->searchIntersection(crv, iPoints, dNum, lineDetection);
		}
		return;
	} //end if


	// two end points are outside the clipping lines

	std::vector<double> uValues;

	auto findUs = [&](double distance)
	{
		if (isThereLineIntersection(distance, pt.y, pt2.y))
		{
			if (debug) { std::cout << "intersection between clipping line and one edge of convex hull of the distance curve: \n"; }
			if (pt.y == pt2.y)
			{
				if (debug)
				{
					std::cout << "   the edge is horizontal (distance curve), u values: \n";
					std::cout << pt.x << ' ' << pt2.x << '\n';
				}
				uValues.push_back(pt.x);
				uValues.push_back(pt2.x);
			}
			else
			{
				uValues.push_back((pt2.x - pt.x) * (distance - pt.y) / (pt2.y - pt.y) + pt.x);
				if (debug) { std::cout << "   u: " << uValues.back() << '\n'; }
			}
		}
	}; // end of findUs

	for (size_t i{}; i < distanceCurve.convexHull.size() - 1; ++i)
	{
		pt = distanceCurve.convexHull[i];
		pt2 = distanceCurve.convexHull[i + 1];
		findUs(maxDist);
		findUs(minDist);
	}

	pt = pt2;
	pt2 = distanceCurve.convexHull.front();
	findUs(maxDist);
	findUs(minDist);

	if (uValues.size() < 2)
	{
		if (debug) { std::cout << "the number of clipped u values < 2, returning\n"; }
		return;
	}

	double u_min{ *std::min_element(uValues.begin(), uValues.end()) };
	double u_max{ *std::max_element(uValues.begin(), uValues.end()) };

	if (std::isnan(u_min) || std::isnan(u_max))
	{
		if (debug) { std::cout << "u value is NaN, returning\n"; }
		return;
	}

	if (std::isinf(u_min) || std::isinf(u_max))
	{
		if (debug) { std::cout << "u vaue is Inf, returning\n"; }
		return;
	}

	if (u_max == u_min)
	{
		if (debug) { std::cout << "The interval of the u on the clipped curve is zero: a point\n"; }
	}

	if (debug) { std::cout << std::format("u_min: {}, u_max: {}\n", u_min, u_max); }

	if ((u_max - u_min) > deltaU2 * 0.8)
	{
		auto bs1{ crv.decompose(crv.knotVector[0], crv.knotVector[0] + deltaU2 / 2.0) };
		if (bs1)
		{
			++dNum;
			bs1->searchIntersection(*this, iPoints, dNum, lineDetection);
		}
		auto bs2{ crv.decompose(crv.knotVector[0] + deltaU2 / 2.0, crv.knotVector[crv.cp_n + 1]) };
		if (bs2) {
			++dNum;
			bs2->searchIntersection(*this, iPoints, dNum, lineDetection);
		}
		return;
	}
	else
	{
		auto decomposedC2{ crv.decompose(u_min, u_max) };
		if (decomposedC2)
		{
			++dNum;
			decomposedC2->searchIntersection(*this, iPoints, dNum, lineDetection);
		}
		return;
	}
} //end searchIntersection


// decompose each curve on knot and find intersection for combination
void Bspline::bezierIntersection(Bspline bs, std::vector<Point>& iPoints, int& dNum, bool lineDetection)
{
	if (!checkNumbers() || !bs.checkNumbers())
	{
		std::cout << "m = n + p + 1 not satisfied\n";
		return;
	}

	std::vector<std::optional<Bspline>> bezierLists[2];

	for (int i{ p_degree }; i <= cp_n; ++i)
	{

		bezierLists[0].push_back(decompose(knotVector[i], knotVector[i + 1]));
		++dNum;

	}

	for (int i{ bs.p_degree }; i <= bs.cp_n; ++i)
	{
		
		bezierLists[1].push_back(bs.decompose(bs.knotVector[i], bs.knotVector[i + 1]));
		++dNum;
	}


	for (size_t i{}; i < bezierLists[0].size(); ++i) {
		for (size_t j{}; j < bezierLists[1].size(); ++j)
		{
			if (bezierLists[0][i].has_value() && bezierLists[1][j].has_value())
				bezierLists[0][i]->searchIntersection(*bezierLists[1][j], iPoints, dNum, lineDetection);
		}
	}

	std::cout << '\t' << dNum << " decomposition(s)\n";

	if (iPoints.size() == 0)
	{
		std::cout << "No intersection\n";
	}
	else
	{
		for (int i{}; i < iPoints.size(); ++i)
		{
			std::cout << std::format("***intersection point #{}: ", i + 1);
			std::cout << iPoints[i] << '\n';
		}
	}
} //end bezierIntersection


bool Bspline::isThereLineIntersection(double y, double y1, double y2) const
{
	if ((y1 > y && y2 > y) || (y1 < y && y2 < y))
		return false;
	else
		return true;
}

bool Bspline::isPointOnLineSegment(const Point& pt, const Bspline& line) const
{
	// find the intersection point between the given line and the perpendicular line passing through the point
	double x{ line.coef_b * line.coef_b * pt.x - line.coef_a * line.coef_b * pt.y - line.coef_a * line.coef_c };
	double y{ line.coef_a * line.coef_a * pt.y - line.coef_a * line.coef_b * pt.x - line.coef_b * line.coef_c };

	auto min_x{ std::min(line.controlPoints.front().x, line.controlPoints.back().x) };
	auto max_x{ std::max(line.controlPoints.front().x, line.controlPoints.back().x) };
	auto min_y{ std::min(line.controlPoints.front().y, line.controlPoints.back().y) };
	auto max_y{ std::max(line.controlPoints.front().y, line.controlPoints.back().y) };

	// check A: the intersection point coinsides with the given point
	// B: the point is inside the line segment
	// due to floating point error check again the point is on either end points of the line segment

	if (
		((std::abs(pt.x - x) < Point::epsilon && std::abs(pt.y - y) < Point::epsilon) &&
			min_x <= x && x <= max_x && min_y <= y && y <= max_y) ||
		pt.hasSameCoordWithTolerance(line.controlPoints.front()) ||
		pt.hasSameCoordWithTolerance(line.controlPoints.back())
		)
	{
		return true;
	}

	return false;
}

void Bspline::printInfo()
{
	std::cout << "---start curve info----------------------------------------------------------------------\n";
	std::cout << "degree: " << p_degree << '\n';
	int i;
	std::cout << "knot vector\n";
	for (const auto& x : knotVector)
		std::cout << " " << x;

	std::cout << '\n';
	std::cout << "number of control points: " << (cp_n + 1) << '\n';
	i = 0;
	for (const auto& pt : controlPoints)
	{
		std::cout << i++ << '\t' << pt << '\n';
	}

	findConvexHull();
	if (!convexHull.empty()) {
		std::cout << "Convex Hull\n";
		for (i = 0; i < convexHull.size(); ++i)
			std::cout << "   " << convexHull[i];
	}

	std::cout << '\n';
	std::cout << "---end curve info------------------------------------------------------------------------\n";
}
