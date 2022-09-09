// September 3, 2022
// by SM
// curve intersection

#include <iostream>
#include <vector>
#include <chrono>
#include <cstring>
#include <SFML/Graphics.hpp>

import Bspline;
import Point;

int main(int argc, char** argv)
{
	/*if (argc < 2)
	{
		std::cout << "Usage: command -e | -b\n";
		return 1;
	}*/

	Bspline curve1{ 3 };

	/*curve1.addPointAndKnots(Point(25, 182));
	curve1.addPointAndKnots(Point(41, 182));
	curve1.addPointAndKnots(Point(44, 194));
	curve1.addPointAndKnots(Point(34, 204));
	curve1.addPointAndKnots(Point(33, 218));
	curve1.addPointAndKnots(Point(43, 232));
	curve1.addPointAndKnots(Point(60, 232));
	curve1.addPointAndKnots(Point(76, 221));
	curve1.addPointAndKnots(Point(77, 208));
	curve1.addPointAndKnots(Point(69, 199));
	curve1.addPointAndKnots(Point(63, 189));
	curve1.addPointAndKnots(Point(68, 181));
	curve1.addPointAndKnots(Point(79, 178));
	curve1.addPointAndKnots(Point(87, 184));
	curve1.addPointAndKnots(Point(92, 199));
	curve1.addPointAndKnots(Point(99, 213));
	curve1.addPointAndKnots(Point(118, 212));
	curve1.addPointAndKnots(Point(130, 200));
	curve1.addPointAndKnots(Point(134, 187));
	curve1.addPointAndKnots(Point(127, 177));
	curve1.addPointAndKnots(Point(114, 176));
	curve1.addPointAndKnots(Point(102, 172));
	curve1.addPointAndKnots(Point(94, 166));
	curve1.addPointAndKnots(Point(90, 158));
	curve1.addPointAndKnots(Point(96, 148));
	curve1.addPointAndKnots(Point(107, 145));
	curve1.addPointAndKnots(Point(112, 153));
	curve1.addPointAndKnots(Point(120, 160));
	curve1.addPointAndKnots(Point(131, 159));
	curve1.addPointAndKnots(Point(142, 149));
	curve1.addPointAndKnots(Point(147, 132));
	curve1.addPointAndKnots(Point(145, 117));
	curve1.addPointAndKnots(Point(133, 103));
	curve1.addPointAndKnots(Point(122, 100));
	curve1.addPointAndKnots(Point(111, 109));
	curve1.addPointAndKnots(Point(102, 121));
	curve1.addPointAndKnots(Point(92, 122));
	curve1.addPointAndKnots(Point(88, 114));
	curve1.addPointAndKnots(Point(86, 106));
	curve1.addPointAndKnots(Point(91, 99));
	curve1.addPointAndKnots(Point(105, 92));
	curve1.addPointAndKnots(Point(115, 75));
	curve1.addPointAndKnots(Point(111, 61));
	curve1.addPointAndKnots(Point(93, 49));
	curve1.addPointAndKnots(Point(80, 51));
	curve1.addPointAndKnots(Point(69, 59));
	curve1.addPointAndKnots(Point(66, 73));
	curve1.addPointAndKnots(Point(68, 85));
	curve1.addPointAndKnots(Point(63, 95));
	curve1.addPointAndKnots(Point(52, 100));
	curve1.addPointAndKnots(Point(44, 98));
	curve1.addPointAndKnots(Point(37, 91));
	curve1.addPointAndKnots(Point(39, 73));
	curve1.addPointAndKnots(Point(39, 59));
	curve1.addPointAndKnots(Point(25, 47));
	curve1.addPointAndKnots(Point(14, 56));
	curve1.addPointAndKnots(Point(10, 68));
	curve1.addPointAndKnots(Point(10, 83));
	curve1.addPointAndKnots(Point(17, 94));
	curve1.addPointAndKnots(Point(26, 103));
	curve1.addPointAndKnots(Point(28, 116));
	curve1.addPointAndKnots(Point(24, 130));
	curve1.addPointAndKnots(Point(21, 142));*/

	/*curve1.addPointAndKnots(Point{ 555, 305 });
	curve1.addPointAndKnots(Point{ 465, 245 });
	curve1.addPointAndKnots(Point{ 400, 255 });
	curve1.addPointAndKnots(Point{ 345, 295 });
	curve1.addPointAndKnots(Point{ 330, 365 });
	curve1.addPointAndKnots(Point{ 340, 425 });
	curve1.addPointAndKnots(Point{ 315, 475 });*/

	curve1.addPointAndKnots(Point{ 350, 300 });
	curve1.addPointAndKnots(Point{ 400, 300 });
	curve1.addPointAndKnots(Point{ 450, 300 });
	curve1.addPointAndKnots(Point{ 500, 300 });
	curve1.addPointAndKnots(Point{ 550, 300 });

	/*curve1.addPointAndKnots(Point{ 200, 200 });
	curve1.addPointAndKnots(Point{ 200, 300 });
	curve1.addPointAndKnots(Point{ 300, 300 });
	curve1.addPointAndKnots(Point{ 200, 200 });*/

	curve1.printInfo();


	/*std::vector<std::optional<Bspline>> curve1_segments;

	for (double u1{0.0}; u1 + 1.0/6.0 < 1.0; u1 += 1.0/6.0)
		curve1_segments.push_back(curve1.decompose(u1, u1 + 1.0/6.0));*/

	//std::vector<std::optional<Bspline>> curve1_test;

	/*curve1_test.push_back(curve1.decompose(0.0, 0.0));
	curve1_test.push_back(curve1.decompose(0.5, 0.5));
	curve1_test.push_back(curve1.decompose(0.7, 0.7));
	curve1_test.push_back(curve1.decompose(1.0, 1.0));*/
	//curve1_test.push_back(curve1.decompose(0.0, 0.5));
	//curve1_test.push_back(curve1.decompose(0.0, 0.7));
	//curve1_test.push_back(curve1.decompose(0.5, 0.7));
	//curve1_test.push_back(curve1.decompose(0.5, 1.0));
	//curve1_test.push_back(curve1.decompose(0.8, 0.9));
	//std::optional<Bspline> curve1_point{curve1.decompose(0.35, 0.35)};
	//if (curve1_point) curve1_point->findConvexHull();
	//std::optional<Bspline> curve1_point2{ curve1.decompose(0.0, 0.0) };
	//if (curve1_point2) curve1_point2->findConvexHull();
	//std::optional<Bspline> curve1_point3{ curve1.decompose(0.5, 0.5) };
	//if (curve1_point3) curve1_point3->findConvexHull();
	//std::optional<Bspline> curve1_point4{ curve1.decompose(1.0, 1.0) };
	//if (curve1_point4) curve1_point4->findConvexHull();

	//Point x;
	//curve1_point4->curvePoint(1.0, x);
	//std::cout << x << '\n';

	Bspline curve2{ 3 };

	/*curve2.addPointAndKnots(Point(79, 248));
	curve2.addPointAndKnots(Point(69, 242));
	curve2.addPointAndKnots(Point(62, 229));
	curve2.addPointAndKnots(Point(64, 215));
	curve2.addPointAndKnots(Point(74, 202));
	curve2.addPointAndKnots(Point(89, 195));
	curve2.addPointAndKnots(Point(103, 192));
	curve2.addPointAndKnots(Point(118, 192));
	curve2.addPointAndKnots(Point(136, 187));
	curve2.addPointAndKnots(Point(151, 175));
	curve2.addPointAndKnots(Point(159, 154));
	curve2.addPointAndKnots(Point(161, 132));
	curve2.addPointAndKnots(Point(157, 120));
	curve2.addPointAndKnots(Point(143, 119));
	curve2.addPointAndKnots(Point(128, 116));
	curve2.addPointAndKnots(Point(117, 111));
	curve2.addPointAndKnots(Point(103, 103));
	curve2.addPointAndKnots(Point(93, 95));
	curve2.addPointAndKnots(Point(86, 83));
	curve2.addPointAndKnots(Point(81, 64));
	curve2.addPointAndKnots(Point(78, 43));
	curve2.addPointAndKnots(Point(70, 34));
	curve2.addPointAndKnots(Point(57, 29));
	curve2.addPointAndKnots(Point(45, 28));*/

	/*curve2.addPointAndKnots(Point{ 465, 475 });
	curve2.addPointAndKnots(Point{ 430, 415 });
	curve2.addPointAndKnots(Point{ 405, 320 });
	curve2.addPointAndKnots(Point{ 390, 215 });
	curve2.addPointAndKnots(Point{ 350, 170 });
	curve2.addPointAndKnots(Point{ 285, 145 });
	curve2.addPointAndKnots(Point{ 225, 140 });*/

	curve2.addPointAndKnots(Point{ 50, 300 });
	curve2.addPointAndKnots(Point{ 100, 300 });
	curve2.addPointAndKnots(Point{ 150, 300 });
	curve2.addPointAndKnots(Point{ 200, 300 });
	curve2.addPointAndKnots(Point{ 250, 300 });

	/*curve2.addPointAndKnots(Point{ 555, 305 });
	curve2.addPointAndKnots(Point{ 465, 245 });
	curve2.addPointAndKnots(Point{ 400, 255 });
	curve2.addPointAndKnots(Point{ 345, 295 });
	curve2.addPointAndKnots(Point{ 330, 365 });
	curve2.addPointAndKnots(Point{ 340, 425 });
	curve2.addPointAndKnots(Point{ 315, 475 });*/

	/*curve2.addPointAndKnots(Point{ 200, 210 });
	curve2.addPointAndKnots(Point{ 200, 310 });
	curve2.addPointAndKnots(Point{ 300, 310 });
	curve2.addPointAndKnots(Point{ 200, 210 });*/

	curve2.printInfo();

	/*std::vector<std::optional<Bspline>> curve2_segments;

	for (double u1{ 0.0 }; u1 + 1.0 / 6.0 < 1.0; u1 += 1.0 / 6.0)
		curve2_segments.push_back(curve2.decompose(u1, u1 + 1.0 / 6.0));*/
	
	int decompNum{};

	std::vector<Point> ptList;
	std::vector<Bspline> splines;
	std::vector<Bspline> components;

	std::chrono::steady_clock::time_point start, stop;

	//if (std::strcmp(argv[1], "e") == 0) {

		//start = std::chrono::high_resolution_clock::now();
		curve1.findIntersection(curve2, ptList, components, decompNum);
		// 
	/*if (curve1_test[0])
	{
		curve1_test[0]->findConvexHull();
		curve1_test[0]->findIntersection(curve2, ptList, components, decompNum);
	}*/
		//stop = std::chrono::high_resolution_clock::now();
		//std::cout << "\n------- Extended Bezier Clipping --------------\n";

	//}
	//else if (std::strcmp(argv[1], "b") == 0) {
		//start = std::chrono::high_resolution_clock::now();
		//curve1.bezierIntersection(curve2, ptList, components, splines, decompNum);
		//stop = std::chrono::high_resolution_clock::now();
		//std::cout << ("\n------- Bezier Clipping --------------\n");
	//}

	/*auto duration = duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << '\t' << duration.count() << " ms\n";*/
	std::cout << '\t' << decompNum << " decomposition(s)\n";

	if (ptList.size() == 0)
	{
		std::cout << "No intersection\n";
	}
	else
	{
		for (int i{}; i < ptList.size(); ++i)
		{
			std::cout << std::format("***intersection point #{}: ", i + 1);
			std::cout << ptList[i] << '\n';
		}
	}

	std::cout << "decomposed spline curves\n";
	std::cout << "number of decomposed curves: " << splines.size() << '\n';
	for (auto& x : splines)
		x.printInfo();

	sf::RenderWindow window(sf::VideoMode(800, 600), "Curve Intersection", sf::Style::Titlebar | sf::Style::Close);


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		

		//curve1.drawControlPolygon(window);
		//curve1.drawConvexHull(window, sf::Color::Yellow);
		curve1.drawCurve(window, sf::Color::Green);

		//curve2.drawControlPolygon(window);
		//curve2.drawConvexHull(window, sf::Color::Red);
		curve2.drawCurve(window, sf::Color::Yellow);

		//sf::Color col;
		//sf::Color col2;
		/*for (size_t i{}; i < curve1_segments.size(); ++i)
		{
			if (i % 2 == 0)
				col = sf::Color::Red;
			else
				col = sf::Color::Yellow;

			if (curve1_segments[i])
			{
				curve1_segments[i]->drawCurve(window, col);
				curve1_segments[i]->drawConvexHull(window, sf::Color::Green);
			}
		}*/

		/*for (size_t i{}; i < curve2_segments.size(); ++i)
		{
			if (i % 2 == 0)
			{
				col = sf::Color::Red;
				col2 = sf::Color::Green;
			}
			else
			{
				col = sf::Color::Yellow;
				col2 = sf::Color::Blue;
			}

			if (curve2_segments[i])
			{
				curve2_segments[i]->drawCurve(window, col);
				curve2_segments[i]->drawConvexHull(window, col2);
			}
		}*/

		/*for (size_t i{}; i < curve1_test.size(); ++i)
		{
			if (i % 2 == 0)
				col = sf::Color::Red;
			else
				col = sf::Color::Yellow;

			if (curve1_test[i])
			{
				Point p;
				curve1_test[i]->curvePoint(curve1_test[i]->knotVector[0], p);
				sf::CircleShape c{ 2 };
				c.setPosition(static_cast<float>(p.x) - c.getRadius(), window.getSize().y - static_cast<float>(p.y) - c.getRadius());
			}
		}*/

		//if (curve1_point4)
		//	curve1_point4->drawCurve(window, sf::Color::Yellow);

		/*for (auto& x : components)
			x.drawCurve(window, sf::Color::Red);

		for (auto& x : splines)
			x.drawCurve(window, sf::Color::Yellow);*/

		auto windowSize{ window.getSize() };

		for (auto& p : ptList)
		{
			sf::CircleShape c{ 2 };
			c.setPosition(static_cast<float>(p.x) - c.getRadius(), windowSize.y - static_cast<float>(p.y) - c.getRadius());
			//c.setOrigin(static_cast<float>(p.x), windowSize.y - static_cast<float>(p.y));
			window.draw(c);
		}

		window.display();
		
		sf::sleep(sf::milliseconds(50));
	}
}
