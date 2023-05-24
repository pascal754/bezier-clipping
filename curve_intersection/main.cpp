// September 3, 2022
// by SM
// curve intersection

import <iostream>;
import <vector>;
import <array>;
import <algorithm>;
import <format>;
import <cstring>;
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

import Bspline;
import Point;
import Grid;
import FileDialog;

auto main() -> int
{
    try
    {
        std::ios_base::sync_with_stdio(false);
        int iteration_num{}; // save the number of decomposition of curves
        int degreeA{ 3 };
        int degreeB{ 3 };
        Bspline curve1{ degreeA };
        Bspline curve2{ degreeB };
        std::vector<Point> ptList;

        sf::RenderWindow window(sf::VideoMode(1000, 800), "Curve Intersection", sf::Style::Titlebar | sf::Style::Close);
        window.setFramerateLimit(60);
        window.setPosition(sf::Vector2i{ 300, 50 });
        ImGui::SFML::Init(window);

        sf::RenderWindow childWindow(sf::VideoMode(290, 790), "Operations", sf::Style::Titlebar);
        childWindow.setFramerateLimit(60);
        ImGui::SFML::Init(childWindow);

        auto pos{ window.getPosition() };
        pos.x += window.getSize().x;

        childWindow.setPosition(pos);

        sf::Clock deltaClock;

        bool curve1EditMode{};
        bool curve2EditMode{};
        bool curve1ConvexHull{};
        bool curve2ConvexHull{};
        //bool curve1ControlPolygon{};
        //bool curve2ControlPolygon{};
        bool gridMode{ true };
        bool gridSnapMode{ true };
        bool decomposeFirst{};
        bool lineDetection{true};
        bool imguiOpen{ true };

        while (window.isOpen())
        {
            auto windowSize{ window.getSize() };
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
                        sf::Vector2i mPos{ sf::Mouse::getPosition(window) };
                        if (gridSnapMode)
                        {
                            mPos.x = static_cast<int>(std::roundf(static_cast<float>(mPos.x) / 50.0f)) * 50;
                            mPos.y = static_cast<int>(std::roundf(static_cast<float>(mPos.y) / 50.0f)) * 50;
                        }
                        std::cout << mPos.x << ' ' << window.getSize().y - mPos.y << std::endl;
                        if (curve1EditMode)
                        {
                            if (curve1.interpolationMode)
                            {
                                curve1.addInterpolationPoint(Point{ static_cast<double>(mPos.x), static_cast<double>(window.getSize().y - mPos.y) });
                            }
                            else
                            {
                                curve1.addPointAndKnots(Point{ static_cast<double>(mPos.x), static_cast<double>(window.getSize().y - mPos.y) });
                            }
                        }
                        if (curve2EditMode)
                        {
                            if (curve2.interpolationMode)
                            {
                                curve2.addInterpolationPoint(Point{ static_cast<double>(mPos.x), static_cast<double>(window.getSize().y - mPos.y) });
                            }
                            else
                            {
                                curve2.addPointAndKnots(Point{ static_cast<double>(mPos.x), static_cast<double>(window.getSize().y - mPos.y) });
                            }
                        }
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
                ImGui::SetNextWindowSize(ImVec2(280, 780));

                ImGui::Begin("Operations", &imguiOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
                ImGui::BeginGroup();

                ImGui::Checkbox("Show grid", &gridMode);
                ImGui::Checkbox("Grid snap", &gridSnapMode);

                ImGui::Separator();
                ImGui::NewLine();

                // launch a file dialog for loading points
                if (ImGui::Button("Load points from the file"))
                {
                    curve1EditMode = false;
                    curve2EditMode = false;
                    curve1ConvexHull = false;
                    curve2ConvexHull = false;
                    /*curve1ControlPolygon = false;
                    curve2ControlPolygon = false;*/

                    std::string filePathName, fileName;
                    if (launchFileDialog('o', filePathName, fileName))
                    {
                        degreeA = 3;
                        degreeB = 3;
                        curve1.changeDegree(degreeA);
                        curve2.changeDegree(degreeB);
                        loadPoints(curve1, curve2, filePathName);
                        ptList.clear();
                    }
                }

                // launch a file dialog for saving points
                if (ImGui::Button("Save points to the file"))
                {
                    curve1EditMode = false;
                    curve2EditMode = false;
                    std::string filePathName, fileName;
                    if (launchFileDialog('s', filePathName, fileName))
                    {
                        savePoints(curve1, curve2, filePathName);
                    }
                }

                ImGui::Separator();
                ImGui::NewLine();

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
                    if (curve1.interpolationMode)
                    {
                        curve1.deleteLastInterpolationPoint();
                    }
                    else
                    {
                        curve1.deleteLastPointAndKnots();
                    }
                }

                if (ImGui::Button("Erase curve A"))
                {
                    curve1EditMode = false;
                    curve2EditMode = false;
                    curve1.clear();
                    ptList.clear();
                }

                //ImGui::Checkbox("Control Polygon for A", &curve1ControlPolygon);
                ImGui::Checkbox("Convex Hull for A", &curve1ConvexHull);

                if (ImGui::InputInt("degree A", &degreeA))
                {
                    curve1EditMode = false;
                    curve2EditMode = false;
                    degreeA = std::clamp(degreeA, 1, 10);
                    curve1.changeDegree(degreeA);
                    ptList.clear();
                }

                if (curve1.getInterpolationPointSize() != 0 || curve1.getControlPointSize() != 0) ImGui::BeginDisabled();
                ImGui::Checkbox("Interpolation Mode for A", &curve1.interpolationMode);
                if (curve1.getInterpolationPointSize() != 0 || curve1.getControlPointSize() != 0) ImGui::EndDisabled();

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
                    if (curve2.interpolationMode)
                    {
                        curve2.deleteLastInterpolationPoint();
                    }
                    else
                    {
                        curve2.deleteLastPointAndKnots();
                    }
                }

                if (ImGui::Button("Erase curve B"))
                {
                    curve1EditMode = false;
                    curve2EditMode = false;
                    curve2.clear();
                    ptList.clear();
                }

                //ImGui::Checkbox("Control Polygon for B", &curve2ControlPolygon);
                ImGui::Checkbox("Convex Hull for B", &curve2ConvexHull);

                if (ImGui::InputInt("degree B", &degreeB))
                {
                    curve1EditMode = false;
                    curve2EditMode = false;
                    degreeB = std::clamp(degreeB, 1, 10);
                    curve2.changeDegree(degreeB);
                    ptList.clear();
                }

                if (curve2.getInterpolationPointSize() != 0 || curve2.getControlPointSize() != 0) ImGui::BeginDisabled();
                ImGui::Checkbox("Interpolation Mode for B", &curve2.interpolationMode);
                if (curve2.getInterpolationPointSize() != 0 || curve2.getControlPointSize() != 0) ImGui::EndDisabled();


                ImGui::Separator();
                ImGui::NewLine();

                ImGui::Checkbox("Decompose curves first", &decomposeFirst);

                ImGui::Checkbox("Point and Line detection", &lineDetection);

                ImGui::Checkbox("Save to `calc.log`\n(last execution only)", &Bspline::DEBUG);

                if (ImGui::Button("Find intersection"))
                {
                    curve1EditMode = false;
                    curve2EditMode = false;
                    curve1ConvexHull = false;
                    curve2ConvexHull = false;
                    /*curve1ControlPolygon = false;
                    curve2ControlPolygon = false;*/

                    window.clear();
                    if (gridMode)
                    {
                        drawGrids(window, windowSize.x, windowSize.y, 50);
                    }
                    curve1.drawCurve(window, sf::Color::Green);
                    curve2.drawCurve(window, sf::Color::Yellow);
                    window.display();

                    iteration_num = 0;
                    ptList.clear();

                    if (Bspline::DEBUG)
                    {
                        Bspline::logFile.open("calc.log", std::ios::out);
                        if (!Bspline::logFile.is_open())
                        {
                            std::cerr << "file open error\n";
                            Bspline::DEBUG = false;
                        }
                    }

                    if (decomposeFirst)
                    {
                        bezierIntersection(curve1, curve2, ptList, iteration_num, lineDetection);
                    }
                    else
                    {
                        findIntersection(curve1, curve2, ptList, iteration_num, lineDetection);
                    }

                    if (Bspline::DEBUG)
                    {
                        Bspline::logFile.close();
                    }
                }

                ImGui::Separator();
                ImGui::NewLine();

                static std::array<char, 512> text{ "Find intersection points\nbetween two B-spline curves\nusing Bezier clipping" };

                //static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly;
                //ImGui::CheckboxFlags("ImGuiInputTextFlags_ReadOnly", &flags, ImGuiInputTextFlags_ReadOnly);
                //ImGui::CheckboxFlags("ImGuiInputTextFlags_AllowTabInput", &flags, ImGuiInputTextFlags_AllowTabInput);
                //ImGui::CheckboxFlags("ImGuiInputTextFlags_CtrlEnterForNewLine", &flags, ImGuiInputTextFlags_CtrlEnterForNewLine);
                ImGui::InputTextMultiline("##source", text.data(), text.size(), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_ReadOnly);

                ImGui::EndGroup();
                ImGui::End();
            }

            window.clear();


            if (gridMode)
            {
                drawGrids(window, windowSize.x, windowSize.y, 50);
            }

            curve1.drawCurve(window, sf::Color::Green);

            /*if (curve1ControlPolygon)
            {
                curve1.drawControlPolygon(window);
            }*/

            if (curve1ConvexHull)
            {
                curve1.drawConvexHull(window, sf::Color::Cyan);
            }

            curve2.drawCurve(window, sf::Color::Yellow);

            /*if (curve2ControlPolygon)
            {
                curve2.drawControlPolygon(window);
            }*/
            if (curve2ConvexHull)
            {
                curve2.drawConvexHull(window, sf::Color::Magenta);
            }



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
        }

        ImGui::SFML::Shutdown();
    }
    catch (const std::exception& e)
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
