// October 1997
// by SM
// filename: Bspline.cpp

module;

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <chrono>
#include <algorithm>
#include <format>
#include <SFML/Graphics.hpp>

module Bspline;


const double Bspline::epsilon{ 1e-5 };
const double Bspline::intersection_epsilon{ 0.00001 };

int Bspline::findKnotSpan(double u)
{
	if (u < knotVector.front() || u > knotVector.back())
	{
		std::cout << "findKnotSpan(): u is outside of range\n";
		exit(1);
	}
	//algorithm A2.1 FindSpan pp68
	if (u == knotVector[cp_n + 1]) // special case // ? (std::abs(u - knotVector[cp_n + 1]) < epsilon) 
		return cp_n;

	int count{};
	int low{ p_degree };
	int high{ cp_n + 1 };
	int mid{ (low + high) / 2 };

	while (u < knotVector[mid] || u >= knotVector[mid + 1]) // sometimes cannot find approximate match; infinite loop (1)
	{
		if (u < knotVector[mid])
			high = mid;
		else
			low = mid;

		mid = (low + high) / 2;

		//if (mid == low) // this solves the problem (1)
			//break;

		++count;
		if (count > 1000)
		{
			std::cout << "trapped in findKnotSpan()\n";
			std::cout << std::format("u: {:.20f}, low: {:.20f}, high: {:.20f}\n", u, knotVector[low], knotVector[high]);
		}
	}

	return mid;

	//auto pos = std::upper_bound(knotVector.begin(), knotVector.end(), u);
	//if (pos != knotVector.end())
	//	return static_cast<int>(std::distance(knotVector.begin(), pos));

	//std::cout << "index not found in findKnotSpan()\n";
	//exit(1);
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

	/* // test -----------------------------------------------------
	for (i=0; i<basis.length; ++i)
	   System.out.println("basis " + i + " " + basis[i]);
	// end test -------------------------------------------------
	*/

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
}

void Bspline::deleteLastPointAndKnots()
{
	deleteLastPoint();
	makeKnots();
}

Point Bspline::getPoint(int i)
{
	return controlPoints[i];
}

int Bspline::findFirstPointOfConvexHull()
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
			std::cout << "in findFirstPointOfConvexHull(): two points have the same y values\n";
			pos = i;
		}
	}
	return static_cast<int>(pos);
}

void Bspline::findConvexHull()
{
	if (controlPoints.size() == 0)
	{
		convexHull.clear();
		return;
	}

	if (controlPoints.size() == 1) // ? needs more thoughts ...
	{
		convexHull = controlPoints;
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

	/* //test ----------------------------------------------------------
	System.out.println("Unsorted Points in method findConvexHull");
	for (i=0; i<unsortedPoints.getNumOfElements(); i++)
	   System.out.println((Point)unsortedPoints.getElement(i));
	//end test ---------------------------------------------------------
	*/

	std::list<Point>::iterator min, index;
	std::vector<Point> sortedPoints, tempConvexHull;

	// sort points by polar angle with the first point
	// if several points have the same polar angle then keep the farthest point from the first point
	while ((unsortedPoints.size()) >= 2) {
		min = unsortedPoints.begin();
		index = std::next(min); // next element
		while (index != unsortedPoints.end()) {
			if (min->angle > index->angle) {
				min = index;
			}
			else if (min->angle == index->angle) // (std::abs(min->angle - index->angle) < epsilon)
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

		/* //test -------------------------------------------------------------
		System.out.println("Sorted Point");
		System.out.println(min);
		//end test ---------------------------------------------------------
		*/

		sortedPoints.push_back(*min);
		unsortedPoints.erase(min);

		/* // test -------------------------------------------------------------
		int t;
		System.out.println("# of Unsorted Point(s): " + unsortedPoints.getNumOfElements());
		for (t=0; t<unsortedPoints.getNumOfElements(); t++)
		   System.out.println((Point)unsortedPoints.getElement(t));
		// end test ----------------------------------------------------------
		*/
	}

	if (unsortedPoints.size() == 1)
		sortedPoints.push_back(unsortedPoints.back());

	/* //test  -------------------------------------------------------------------
	int t;
	System.out.println("# of Sorted Point(s): " + sortedPoints.getNumOfElements());
	System.out.println("Elements of Sorted Points");
	for (t=0; t<sortedPoints.getNumOfElements(); t++)
	   System.out.println((Point)sortedPoints.getElement(t));
	// end test ----------------------------------------------------------------
	*/

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
} //end findConvexHull

bool Bspline::leftTurn(const Point& p0, const Point& p1, const Point& p2)
{
	if ((p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y) > 0)
		return true;
	else
		return false;
}


int Bspline::findKnotMult(int i)
{
	// count multiplicity of knot

	int multiple{ 1 };
	if (i == 0)
		return multiple;

	while (i > 0 && knotVector[i] == knotVector[i - 1]) // std::abs(knotVector[i] - knotVector[i - 1]) < epsilon)
	{
		++multiple;
		--i;
	}

	return multiple;
}


bool Bspline::hasEnoughPoints()
{
	return (cp_n + 1 > p_degree) ? true : false;
}

void Bspline::makeKnots()
{ // makes uniform internal knots
	// n + 1 > p
	// n + 1 : number of control points:
	// m: n + 1 + p (last index)
	// p_degree: degree of the curve

	if (!hasEnoughPoints())
	{
		std::cout << "makeKnots() : not enough control points to make knot vector\n";
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

double Bspline::getMinPara()
{
	return knotVector[0];
}

double Bspline::getMaxPara() {
	return knotVector[cp_n + 1];
}

void Bspline::drawControlPolygon(sf::RenderWindow& window)
{
	if (cp_n < 0)
		return;

	//int i;
	//Point pt1, pt2;
	sf::VertexArray poly{ sf::LineStrip };
	auto wSize{ window.getSize() };
	for (const auto& pt : controlPoints)
		poly.append(sf::Vertex{ sf::Vector2f{ static_cast<float>(pt.x), wSize.y - static_cast<float>(pt.y) } });

	window.draw(poly);

	/*pt2 = controlPoints[0]
	g.drawRect((int)pt2.x - 2, h - (int)pt2.y - 2, 4, 4);

	for (i = 1; i <= cp_n; ++i) {
		pt1 = pt2;
		pt2 = controlPoints[i];
		g.drawRect((int)pt2.x - 2, h - (int)pt2.y - 2, 4, 4);
		g.drawLine((int)pt1.x, h - (int)pt1.y, (int)pt2.x, h - (int)pt2.y);
	}*/
}

//void Bspline::drawCurveSegments(Graphics g, int h)
//{
//	if (!hasEnoughPoints())
//		return;
//
//	Point pt = new Point();
//
//    for (int i{ p }; i <= cp_n + 1; ++i) {
//		curvePoint(knotVector[i], pt);
//		g.fillRect((int)pt.x - 2, h - (int)pt.y - 2, 4, 4);
//	}
//}


void Bspline::drawCurve(sf::RenderWindow& window, sf::Color col)
{
	if (cp_n >= p_degree)
	{
		double step = (knotVector[cp_n + 1] - knotVector[0]) / ((cp_n / 20 + 1) * 200);
		Point pt; // 1, pt2;
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


std::optional<Bspline> Bspline::decompose(double u1, double u2)
{
	// Algorithm A5.4 RefineKnotVectCurve on pp164

	//if  (std::abs(u1 - u2) < epsilon) // (u1 == u2) // if decomposition skipped, intersection points might be missed.
	//{
	//	std::cout << std::format("in decompose(): u1 and u2 are very close (< {}), decomposition skipped\n", epsilon);
	//	return {};
	//}

	std::cout << std::format("in decompisition u1: {}, u2: {}\n", u1, u2);

	if (u1 < 0 || u2 < 0)
	{
		std::cout << "in decompose(): negative knot, return\n";
		return {};
	}

	if (u1 < knotVector.front() || u2 > knotVector.back())
	{
		std::cout << "in decompose: interval is outside the knot vector, return\n";
		return {};
	}


	if (static_cast<int>(knotVector.size()) - 1 != cp_n + p_degree + 1)
	{
		std::cout << "in decompose(); m = n + p + 1 not satisfied, return\n";
		return {};
	}

	if (u1 == knotVector[0] && u2 == knotVector[cp_n + 1]) // (std::abs(u1 - knotVector[0]) < epsilon && std::abs(u2 - knotVector[cp_n + 1]) < epsilon)
	{
		std::cout << "in decompose(): self decomposition, return\n";
		return *this;
	}

	int pre_a, pre_b; // a, b: indices in knotVector
	int m1, m2; // multiplicity for u1 and u2, respectively
	m1 = m2 = p_degree + 1;

	pre_a = findKnotSpan(u1);
	pre_b = findKnotSpan(u2);

	if (u1 == knotVector[pre_a]) // (std::abs(u1 - knotVector[pre_a]) < epsilon)
	{
		m1 = findKnotMult(pre_a);
		m1 = p_degree + 1 - m1;
	}

	if (u2 == knotVector[cp_n + 1]) // (std::abs(u2 - knotVector[cp_n + 1]) < epsilon)
	{
		m2 = 0;
	}
	else if (pre_a != pre_b && u2 == knotVector[pre_b]) // (std::abs(u2 - knotVector[pre_b]) < epsilon)
	{
		m2 = findKnotMult(pre_b);
		m2 = p_degree + 1 - m2;
	}

	if (m1 > p_degree + 1 || m1 < 0 || m2 > p_degree + 1 || m2 < 0)
	{
		std::cout << "in decompose(): knot vector is wrong\n";
		return {};
	}

	++pre_b;

	int sum{ m1 + m2 };
	if (sum == 0)
	{
		std::cout << "in decompose(): no knot insertion, return\n";
		return {};
	}
	std::vector<double> uBar;
	std::vector<Point> qw;

	std::cout << std::format("in decompose() pre_a: {}, pre_b: {}, m1: {}, m2: {}\n", pre_a, pre_b, m1, m2);


	std::vector<double> X;
	for (int j{}; j < m1; ++j)
		X.push_back(u1);

	for (int j{ m1 }; j < sum; ++j)
		X.push_back(u2);

	// find new indices for X vector
	int post_a{ findKnotSpan(X.front()) };
	int post_b{ findKnotSpan(X.back()) };

	++post_b;

	std::cout << std::format("in decompose() post_a: {}, post_b: {}\n", post_a, post_b);

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
			if (alpha == 0.0) // (std::abs(alpha) < epsilon)
			{
				qw[ind - 1] = qw[ind];
			}
			else
			{
				if (ind >= static_cast<int>(qw.size()))
				{
					std::cout << "in decompose(): index out of range\n";
					exit(1);
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

	if (u2 == knotVector[cp_n + 1]) // (std::abs(u2 - knotVector[cp_n + 1]) < epsilon)
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
			"in decompose(): new knot is negative\n";
			return {};
		}
	}


	Bspline decomposed{ p_degree, newKnots };
	size_t num_control_points{ newKnots.size() - p_degree - 1 };
	for (size_t j{ 0 }; j < num_control_points; ++j)
	{
		decomposed.addPoint(qw[j + bias]);
	}

	if (static_cast<int>(decomposed.knotVector.size()) - 1 != decomposed.cp_n + decomposed.p_degree + 1)
	{
		std::cout << "in decompose(): decomposed curve error; m = n + p + 1 not satisfied, return\n";
		return {};
	}

	return decomposed;
} //end decompose

void Bspline::findLineThruEndPoints()
{
	Point pt0{ controlPoints.front() };
	Point ptn{ controlPoints.back() };
	if (pt0.hasSameCoord(ptn))
	{
		std::cout << "controlpoint[0] coincides with controlPoint[n]\n";
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

void Bspline::findIntersection(Bspline crv, std::vector<Point>& iPoints, std::vector<Bspline>& splines, int& dNum)
//void Bspline::findIntersection(Bspline crv, std::vector<Point> iPoints, std::vector<int> dNum)
{
	if (dNum > 1'000'000)
	{
		std::cout << "maximum number of decomposition reached(1,000,000), return\n";
		return;
	}

	if (iPoints.size() > 1'000'000)
	{
		std::cout << "The number of intersection points reached(1,000,000), return\n";
		return;
	}

	std::cout << std::format("in findIntersection() #{}\n", dNum);
	std::cout << std::format("curve1 u1: {}, u2: {}\n", knotVector.front(), knotVector.back());
	std::cout << std::format("curve2 u1: {}, u2: {}\n", crv.knotVector.front(), crv.knotVector.back());
	double deltaU1{ knotVector[cp_n + 1] - knotVector[0] };
	double deltaU2{ crv.knotVector[crv.cp_n + 1] - crv.knotVector[0] };


	if (deltaU1 < 0 || deltaU2 < 0)
	{
		std::cout << "in findIntersection(): negative delta u, return\n";
		return;
	}

	if (deltaU1 < intersection_epsilon && deltaU2 < intersection_epsilon)
	{

		std::cout << "deltaU1 and deltaU2 are within tolerance: candidate for intersection\n";
		Point intersectPt, intersectPt2;
		curvePoint(knotVector[0] + deltaU1 / 2.0, intersectPt);
		crv.curvePoint(crv.knotVector[0] + deltaU2 / 2.0, intersectPt2);

		std::cout << std::format("u on curve A: {}, u on curve B: {}\n", knotVector[0] + deltaU1 / 2.0, crv.knotVector[0] + deltaU2 / 2.0);
		std::cout << std::format("a candidate point on curve A: ({}, {})\n", intersectPt.x, intersectPt.y);
		std::cout << std::format("a candidate point on curve B: ({}, {})\n", intersectPt2.x, intersectPt2.y);

		if (std::abs(intersectPt.x - intersectPt2.x) < 0.05 && std::abs(intersectPt.y - intersectPt2.y) < 0.05)
		{
			std::cout << "=== Intersection found === \n";
			iPoints.push_back(intersectPt);
		}
		else
		{
			std::cout << "Two points are not within tolerance. no intersection\n";
		}
		return;
	}

	if (deltaU1 < epsilon)
	{
		if (deltaU1 == 0)
		{
			std::cout << "in findIntersection(): one curve became a point. return\n";
			{
				printInfo();
				crv.printInfo();
				return;
			}
		}

		std::cout << "one curve became a point. decompose the other.\n";

		Point pt1, pt2, pt3;
		curvePoint(knotVector[0] + deltaU1 / 2.0, pt1);
		crv.curvePoint(crv.knotVector.front(), pt2);
		crv.curvePoint(crv.knotVector.back(), pt3);

		std::cout << std::format("point on curve A: ({}, {})\n", pt1.x, pt1.y);
		std::cout << std::format("start point on curve B: ({}, {})\n", pt2.x, pt2.y);
		std::cout << std::format("end point on curve B : ({}, {})\n", pt3.x, pt3.y);

		auto bs1{ crv.decompose(crv.knotVector[0], crv.knotVector[0] + deltaU2 / 2.0) };
		if (bs1)
		{
			++dNum;
			splines.push_back(*bs1);
			bs1->findIntersection(*this, iPoints, splines, dNum);
		}

		auto bs2{ crv.decompose(crv.knotVector[0] + deltaU2 / 2.0, crv.knotVector[crv.cp_n + 1]) };
		if (bs2)
		{
			++dNum;
			splines.push_back(*bs2);
			bs2->findIntersection(*this, iPoints, splines, dNum);
		}

		return;
	}

	if (deltaU1 > deltaU2)
	{
		crv.findIntersection(*this, iPoints, splines, dNum);
		return;
	}

	findMinMaxDistance();
	if (maxDist == minDist)  // (std::abs(maxDist - minDist) < epsilon)
	{
		std::cout << "max and min distance have the same value\n";
	}

	Bspline distanceCurve{ Bspline(crv.p_degree, crv.knotVector) };
	Point pt;
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

	if (min > maxDist || max < minDist) // outside the clipping lines: no intersection
		return;

	distanceCurve.findConvexHull();

	if (distanceCurve.controlPoints.size() < 2)
	{
		std::cout << "distanceCurve.controlPoints.size() < 2\n";
		exit(1);
	}

	pt = distanceCurve.controlPoints.front();
	Point pt2{ distanceCurve.controlPoints.back() };

	if ((pt.y >= minDist && pt.y <= maxDist) || (pt2.y >= minDist && pt2.y <= maxDist))  // convex hull is inside the clipping lines
	{
		auto bs1{ decompose(knotVector[0], knotVector[0] + deltaU1 / 2.0) };
		if (bs1)
		{
			++dNum;
			splines.push_back(*bs1);
			bs1->findIntersection(crv, iPoints, splines, dNum);
		}
		auto bs2{ decompose(knotVector[0] + deltaU1 / 2.0, knotVector[cp_n + 1]) };
		if (bs2)
		{
			++dNum;
			splines.push_back(*bs2);
			bs2->findIntersection(crv, iPoints, splines, dNum);
		}
		return;
	} //end if

	std::vector<double> uValues;

	auto findUs = [&](double distance)
	{
		if (isThereLineIntersection(distance, pt.y, pt2.y))
		{
			std::cout << "line intersection: \n";
			if (pt.y == pt2.y) // (std::abs(pt.y - pt2.y) < epsilon)
			{
				std::cout << "in findIntersection(): horizontal line (distance curve)\n";
				std::cout << pt.x << ' ' << pt2.x << '\n';
				uValues.push_back(pt.x);
				uValues.push_back(pt2.x);
			}
			else
			{
				uValues.push_back((pt2.x - pt.x) * (distance - pt.y) / (pt2.y - pt.y) + pt.x);
				std::cout << uValues.back() << '\n';
			}
		}
	}; // end of findUs

	for (size_t i{}; i < distanceCurve.convexHull.size() - 1; ++i)
	{
		pt = distanceCurve.convexHull[i];;
		pt2 = distanceCurve.convexHull[i + 1];
		findUs(maxDist);
		findUs(minDist);
	}

	pt = pt2;
	pt2 = distanceCurve.convexHull.front();
	findUs(maxDist);
	findUs(minDist);

	if (uValues.size() < 2)
		return;

	min = *std::min_element(uValues.begin(), uValues.end());
	max = *std::max_element(uValues.begin(), uValues.end());

	if (std::isnan(min) || std::isnan(max))
	{
		std::cout << "NaN\n";
		return;
	}

	if (std::isinf(min) || std::isinf(max))
	{
		std::cout << "Inf\n";
		return;
	}

	//if (max == min) // (std::abs(max - min) < epsilon)
	//{
	//	std::cout << "interval of the clipped u on a curve is zero.\n";
	//	std::cout << "deltaU1" << deltaU1 << '\n';
	//	if (deltaU1 < intersection_epsilon) // probably will not executed because the range of deltaU1 already checked above.
	//	{
	//		std::cout << "deltaU1 and deltaU2 are within tolerance: candidate for intersection\n";
	//		Point intersectPt, intersectPt2;
	//		curvePoint(knotVector[0] + deltaU1 / 2.0, intersectPt);
	//		crv.curvePoint(min, intersectPt2);
	//		std::cout << std::format("u on curve A: {}, u on curve B: {}\n", knotVector[0] + deltaU1 / 2.0, min);
	//		std::cout << std::format("a candidate point on curve A: ({}, {})\n", intersectPt.x, intersectPt.y);
	//		std::cout << std::format("a candidate point on curve B: ({}, {})\n", intersectPt2.x, intersectPt2.y);
	//		if (std::abs(intersectPt.x - intersectPt2.x) < 0.05 && std::abs(intersectPt.y - intersectPt2.y) < 0.05)
	//		{
	//			std::cout << "=== Intersection found ===\n";
	//			iPoints.push_back(intersectPt);
	//		}
	//		else
	//		{
	//			std::cout << "Two points are not within tolerance. no intersection\n";
	//		}
	//	}
	//	return;
	//}

	std::cout << std::format("min: {}, max: {}\n", min, max);

	if ((max - min) > deltaU2 * 0.8)
	{
		auto bs1{ crv.decompose(crv.knotVector[0], crv.knotVector[0] + deltaU2 / 2.0) };
		if (bs1)
		{
			++dNum;
			splines.push_back(*bs1);
			bs1->findIntersection(*this, iPoints, splines, dNum);
		}
		auto bs2{ crv.decompose(crv.knotVector[0] + deltaU2 / 2.0, crv.knotVector[crv.cp_n + 1]) };
		if (bs2) {
			++dNum;
			splines.push_back(*bs2);
			bs2->findIntersection(*this, iPoints, splines, dNum);
		}
		return;
	}
	else
	{
		auto decomposedC2{ crv.decompose(min, max) };
		if (decomposedC2)
		{
			++dNum;
			decomposedC2->findIntersection(*this, iPoints, splines, dNum);
		}
		return;
	}
} //end findIntersection


void Bspline::bezierIntersection(Bspline bs, std::vector<Point>& iPoints, std::vector<Bspline>& components, std::vector<Bspline>& splines, int& dNum)
{
	std::vector<std::optional<Bspline>> bezierLists[2];

	for (int i{ p_degree }; i <= cp_n; ++i)
	{
		bezierLists[0].push_back(decompose(knotVector[i], knotVector[i + 1]));
		if (bezierLists[0].back())
			components.push_back(*bezierLists[0].back());
		++dNum;
	}

	for (int i{ bs.p_degree }; i <= bs.cp_n; ++i)
	{
		bezierLists[1].push_back(bs.decompose(bs.knotVector[i], bs.knotVector[i + 1]));
		if (bezierLists[1].back())
			components.push_back(*bezierLists[1].back());
		++dNum;
	}


	for (size_t i{}; i < bezierLists[0].size(); ++i) {
		for (size_t j{}; j < bezierLists[1].size(); ++j)
		{
			if (bezierLists[0][i].has_value() && bezierLists[1][j].has_value())
				bezierLists[0][i]->findIntersection(*bezierLists[1][j], iPoints, splines, dNum);
		}
	}
} //end bezierIntersection


bool Bspline::isThereLineIntersection(double y, double y1, double y2)
{
	if ((y1 > y && y2 > y) || (y1 < y && y2 < y))
		return false;
	else
		return true;
}


void Bspline::printInfo()
{
	std::cout << "----------------------------------------------------------------------\n";
	std::cout << "Information of this curve\n";
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

	std::cout << "----------------------------------------------------------------------\n";
}
