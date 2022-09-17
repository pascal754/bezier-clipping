// September 3, 2022
// by SM
// curve intersection

//#include <iostream>
//#include <vector>
//#include <chrono>
//#include <cstring>

import <iostream>;
import <vector>;
import <chrono>;
import <cstring>;

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

		Bspline curve1{ 3 }, curve2{ 3 }; // p, degree of the curves set to 3
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


		

	
		sf::RenderWindow window(sf::VideoMode(1000, 800), "Curve Intersection", sf::Style::Titlebar | sf::Style::Close);
		window.setFramerateLimit(60);
		ImGui::SFML::Init(window);

		sf::RenderWindow childWindow(sf::VideoMode(300, 600), "Operations", sf::Style::Titlebar);
		childWindow.setFramerateLimit(60);
		ImGui::SFML::Init(childWindow);


		sf::Clock deltaClock;

		bool curve1EditMode{}, curve2EditMode{};
		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				ImGui::SFML::ProcessEvent(window, event);

				if (event.type == sf::Event::Closed)
				{
					if (childWindow.isOpen())
					{
						childWindow.close();
					}

					window.close();
				}
				else if (event.type == sf::Event::MouseButtonPressed)
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						sf::Vector2i pos{ sf::Mouse::getPosition(window) };
						std::cout << pos.x << ' ' << pos.y << std::endl;
						if (curve1EditMode)
							curve1.addPointAndKnots(Point{ static_cast<double>(pos.x), static_cast<double>(window.getSize().y - pos.y) });
						if (curve2EditMode)
							curve2.addPointAndKnots(Point{ static_cast<double>(pos.x), static_cast<double>(window.getSize().y - pos.y) });
					}
				}
			}

			// Child window event processing
			if (childWindow.isOpen())
			{
				while (childWindow.pollEvent(event))
				{
					ImGui::SFML::ProcessEvent(childWindow, event);
					if (event.type == sf::Event::Closed)
					{
						childWindow.close();
						ImGui::SFML::Shutdown(childWindow);
					}
				}
			}

			// Update
			const sf::Time dt = deltaClock.restart();

			ImGui::SFML::Update(window, dt);

			if (childWindow.isOpen())
			{
				ImGui::SFML::Update(childWindow, dt);
			}


			//ImGui::ShowDemoWindow();

			// Add ImGui widgets in the child window
			if (childWindow.isOpen()) {

				ImGui::Begin("Operations");
				ImGui::BeginGroup();
				if (ImGui::Button("Add a point to curve A"))
				{
					curve1EditMode = true;
				}
				if (ImGui::Button("Delete the last point from curve A"))
				{
					if (ptList.size() != 0)
					{
						ptList.clear();
					}
					curve1.deleteLastPointAndKnots();
				}
				if (ImGui::Button("Finish curve A edit mode"))
				{
					curve1EditMode = false;
				}
				if (ImGui::Button("Erase curve A"))
				{
					curve1.clear();
				}
				if (ImGui::Button("Add a point to curve B"))
				{
					curve2EditMode = true;
				}
				if (ImGui::Button("Delete the last point from curve B"))
				{
					if (ptList.size() != 0)
					{
						ptList.clear();
					}
					curve2.deleteLastPointAndKnots();
				}
				if (ImGui::Button("Finish curve B edit mode"))
				{
					curve2EditMode = false;
				}
				if (ImGui::Button("Erase curve B"))
				{
					curve2.clear();
				}
				if (ImGui::Button("Find intersection"))
				{
					decomp_num = 0;
					if (ptList.size() != 0)
					{
						ptList.clear();
					}
					curve1.findIntersection(curve2, ptList, decomp_num, false);
				}
				if (ImGui::Button("Decompose and find intersection"))
				{
					decomp_num = 0;
					if (ptList.size() != 0)
					{
						ptList.clear();
					}
					curve1.bezierIntersection(curve2, ptList, decomp_num, false);
				}
				ImGui::EndGroup();
				ImGui::End();
			}

			window.clear();
			
			//curve1.drawCurve(window, sf::Color::Yellow);

			//curve1.drawControlPolygon(window);
			//curve1.drawConvexHull(window, sf::Color::Yellow);
			curve1.drawCurve(window, sf::Color::Green);

			//curve2.drawControlPolygon(window);
			//curve2.drawConvexHull(window, sf::Color::Red);
			curve2.drawCurve(window, sf::Color::Yellow);


			

			auto windowSize{ window.getSize() };

			for (auto& p : ptList)
			{
				sf::CircleShape c{ 3 };
				c.setPosition(static_cast<float>(p.x) - c.getRadius(), windowSize.y - static_cast<float>(p.y) - c.getRadius());
				window.draw(c);
			}

			ImGui::SFML::Render(window);

			window.display();

			// Child window drawing
			if (childWindow.isOpen())
			{
				childWindow.clear();
				ImGui::SFML::Render(childWindow);
				childWindow.display();
			}

			//sf::sleep(sf::milliseconds(100));
		}

		ImGui::SFML::Shutdown();
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
