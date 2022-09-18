// September 3, 2022
// by SM
// curve intersection

#include <iostream>
#include <vector>
#include <chrono>
#include <cstring>

//import <iostream>;
//import <vector>;
//import <chrono>;
//import <cstring>;

#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>


import Bspline;
import Point;
import make_curve;

int main()
{
	try
	{
		int decomp_num{}; // save the number of decomposition of curves

		Bspline curve1{ 3 }, curve2{ 3 }; // p, degree of the curves set to 3
		std::vector<Point> ptList;
	
		sf::RenderWindow window(sf::VideoMode(1000, 800), "Curve Intersection", sf::Style::Titlebar | sf::Style::Close);
		window.setFramerateLimit(60);
		ImGui::SFML::Init(window);

		sf::RenderWindow childWindow(sf::VideoMode(300, 350), "Operations", sf::Style::Titlebar);
		childWindow.setFramerateLimit(60);
		ImGui::SFML::Init(childWindow);

		auto pos{ window.getPosition() };
		pos.x += window.getSize().x;

		childWindow.setPosition(pos);

		sf::Clock deltaClock;

		bool curve1EditMode{};
		bool curve2EditMode{};
		bool curve1ConvexHull{}, curve2ConvexHull{};
		bool decomposeFirst{};
		bool imguiOpen{ true };
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

			// Add ImGui widgets in the child window
			if (childWindow.isOpen()) {

				ImGui::SetNextWindowPos(ImVec2(5, 5)); // , ImGuiCond_FirstUseEver);
				ImGui::SetNextWindowSize(ImVec2(280, 300));

				ImGui::Begin("Operations", & imguiOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
				ImGui::BeginGroup();
				if (ImGui::Button("Add a point to curve A"))
				{
					curve1EditMode = true;
					curve2EditMode = false;
					ptList.clear();
				}
				if (ImGui::Button("Delete the last point from curve A"))
				{
					if (ptList.size() != 0)
					{
						ptList.clear();
					}
					curve1.deleteLastPointAndKnots();
				}

				if (ImGui::Button("Erase curve A"))
				{
					curve1EditMode = false;
					curve2EditMode = false;
					curve1.clear();
					ptList.clear();
				}

				ImGui::Checkbox("Convex Hull for A", &curve1ConvexHull);

				ImGui::Separator();
				ImGui::NewLine();

				if (ImGui::Button("Add a point to curve B"))
				{
					curve1EditMode = false;
					curve2EditMode = true;
					ptList.clear();
				}
				if (ImGui::Button("Delete the last point from curve B"))
				{
					if (ptList.size() != 0)
					{
						ptList.clear();
					}
					curve2.deleteLastPointAndKnots();
				}

				if (ImGui::Button("Erase curve B"))
				{
					curve1EditMode = false;
					curve2EditMode = false;
					curve2.clear();
					ptList.clear();
				}
				
				ImGui::Checkbox("Convex Hull for B", &curve2ConvexHull);

				ImGui::Separator();
				ImGui::NewLine();

				ImGui::Checkbox("Decompose curves first", &decomposeFirst);

				if (ImGui::Button("Find intersection"))
				{
					curve1EditMode = false;
					curve2EditMode = false;
					curve1ConvexHull = false;
					curve2ConvexHull = false;

					window.clear();
					curve1.drawCurve(window, sf::Color::Green);
					curve2.drawCurve(window, sf::Color::Yellow);
					window.display();

					decomp_num = 0;
					if (ptList.size() != 0)
					{
						ptList.clear();
					}
					if (decomposeFirst)
					{
						curve1.bezierIntersection(curve2, ptList, decomp_num, false);
					}
					else
					{
						curve1.findIntersection(curve2, ptList, decomp_num, false);
					}
				}
				ImGui::EndGroup();
				ImGui::End();
			}

			window.clear();
			
			curve1.drawCurve(window, sf::Color::Green);
			//curve1.drawControlPolygon(window);
			if (curve1ConvexHull)
			{
				curve1.drawConvexHull(window, sf::Color::White);
			}

			curve2.drawCurve(window, sf::Color::Yellow);
			//curve2.drawControlPolygon(window);
			if (curve2ConvexHull)
			{
				curve2.drawConvexHull(window, sf::Color::Magenta);
			}


			

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
