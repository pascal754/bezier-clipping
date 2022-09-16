// September 3, 2022
// by SM
// curve intersection

#include <iostream>
#include <vector>
#include <chrono>
#include <cstring>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>


import Bspline;
import Point;
import make_curve;

void printUsage()
{
	std::cout << "Usage: command e | b\n";
}




int main()
{
	try
	{
		int decomp_num{}; // number of iteration for intersection

		Bspline curve1{ 3 }; // , curve2{ 3 }; // p, degree of the curves set to 3
		std::vector<Point> ptList;
		//std::vector<Bspline> splines;
		//
		//make_curve26(curve1, curve2);
		//do_test100();

		//std::chrono::steady_clock::time_point start, stop;

		//start = std::chrono::high_resolution_clock::now();

		//curve1.findIntersection(curve2, ptList, decomp_num, false);
		//curve1.bezierIntersection(curve2, ptList, decomp_num, false);
			
		/*stop = std::chrono::high_resolution_clock::now();
		auto duration = duration_cast<std::chrono::milliseconds>(stop - start);
		std::cout << '\t' << duration.count() << " ms\n";*/


		/*std::cout << '\t' << decomp_num << " decomposition(s)\n";

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
		}*/

	
		sf::RenderWindow window(sf::VideoMode(800, 800), "Curve Intersection", sf::Style::Titlebar | sf::Style::Close);
		window.setFramerateLimit(60);
		ImGui::SFML::Init(window);

		sf::Clock deltaClock;

		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				ImGui::SFML::ProcessEvent(window, event);

				if (event.type == sf::Event::Closed)
				{
					window.close();
				}
				else if (event.type == sf::Event::MouseButtonPressed)
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						sf::Vector2i pos{ sf::Mouse::getPosition(window) };
						std::cout << pos.x << ' ' << pos.y << std::endl;
						curve1.addPointAndKnots(Point{ static_cast<double>(pos.x), static_cast<double>(window.getSize().y - pos.y) });
					}
				}
			}

			ImGui::SFML::Update(window, deltaClock.restart());

			//ImGui::ShowDemoWindow();

			ImGui::Begin("Operations");
			ImGui::BeginGroup();
			if (ImGui::Button("Add a point"))
				std::cout << "Add a point clicked\n";
			if (ImGui::Button("Delete a point"))
				std::cout << "Delete a point clicked\n";
			if (ImGui::Button("Clear the curve"))
				std::cout << "Clear the curve clicked\n";
			ImGui::Button("End");
			ImGui::EndGroup();
			ImGui::End();

			window.clear();
			
			//curve1.drawCurve(window, sf::Color::Yellow);

			//curve1.drawControlPolygon(window);
			//curve1.drawConvexHull(window, sf::Color::Yellow);
			curve1.drawCurve(window, sf::Color::Green);

			//curve2.drawControlPolygon(window);
			//curve2.drawConvexHull(window, sf::Color::Red);
			//curve2.drawCurve(window, sf::Color::Yellow);


			

			/*auto windowSize{ window.getSize() };

			for (auto& p : ptList)
			{
				sf::CircleShape c{ 3 };
				c.setPosition(static_cast<float>(p.x) - c.getRadius(), windowSize.y - static_cast<float>(p.y) - c.getRadius());
				window.draw(c);
			}*/

			ImGui::SFML::Render(window);

			window.display();

			sf::sleep(sf::milliseconds(100));
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	catch (...)
	{
		std::cerr << "something wrong\n";
		return 1;
	}
}
