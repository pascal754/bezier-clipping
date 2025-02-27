// September 3, 2022
// by SM
// curve intersection

// April 10, 2024
// use import std;

// February 27, 2025
// sfml v2 -> v3; breaking changes

#include "pch.h"

import Bspline;
import FileDialog;
import Grid;
import NodeInfo;
import Point;
import std;

auto main() -> int
{
  try
  {
    //std::ios_base::sync_with_stdio(false);
    ParamInfo paramInfo{};
    int degreeA{ 3 };
    int degreeB{ 3 };
    Bspline curve1{ degreeA };
    Bspline curve2{ degreeB };
    curve1.setID(0);
    curve2.setID(1);
    sf::VertexArray va1{ sf::PrimitiveType::LineStrip }; // for curve1.drawCurve()
    sf::VertexArray va2{ sf::PrimitiveType::LineStrip }; // for curve2.drawCurve()

    sf::RenderWindow window{ sf::VideoMode{{1000, 800}}, "Curve Intersection", sf::Style::Titlebar | sf::Style::Close };
    window.setFramerateLimit(60);
    window.setPosition(sf::Vector2i{ 300, 50 });
    if (!ImGui::SFML::Init(window))
    {
      throw std::runtime_error{ "ImGui::SFML::Init error" };
    }

    sf::RenderWindow childWindow{ sf::VideoMode{{290, 790}}, "Operations", sf::Style::Titlebar };
    childWindow.setFramerateLimit(60);
    if (!ImGui::SFML::Init(childWindow))
    {
      throw std::runtime_error{ "ImGui::SFML::Init error" };
    }

    sf::Vector2i pos{ window.getPosition() };
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
    bool imguiOpen{ true };

    while (window.isOpen())
    {
      sf::Vector2u windowSize{ window.getSize() };

      while (const std::optional event{ window.pollEvent() })
      {
        ImGui::SFML::ProcessEvent(window, *event);

        if (event->is<sf::Event::Closed>())
        {
          if (childWindow.isOpen())
          {
            childWindow.close();
          }

          window.close();
        }
        else if (const auto* mousePressed{ event->getIf<sf::Event::MouseButtonPressed>() })
        {
          if (mousePressed->button == sf::Mouse::Button::Left)
          {
            sf::Vector2i mPos{ sf::Mouse::getPosition(window) };
            if (gridSnapMode)
            {
              mPos.x = static_cast<int>(std::roundf(static_cast<float>(mPos.x) / 50.0f)) * 50;
              mPos.y = static_cast<int>(std::roundf(static_cast<float>(mPos.y) / 50.0f)) * 50;
            }
            std::println("{} {}", mPos.x, window.getSize().y - mPos.y);
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
        while (const std::optional event{ childWindow.pollEvent() })
        {
          ImGui::SFML::ProcessEvent(childWindow, *event);
          if (event->is<sf::Event::Closed>())
          {
            childWindow.close();
            ImGui::SFML::Shutdown(childWindow);
          }
        }
      }

      // Update
      const sf::Time dt{ deltaClock.restart() };

      ImGui::SFML::Update(window, dt);

      if (childWindow.isOpen())
      {
        ImGui::SFML::Update(childWindow, dt);
      }

      // Add ImGui widgets in the child window
      if (childWindow.isOpen()) {
        ImGui::SFML::SetCurrentWindow(childWindow); // added 2025. 2.27
        ImGui::SetNextWindowPos(ImVec2{ 5.f, 5.f }); // , ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2{ 280.f, 780.f });

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

          std::string filePathName;
          std::string fileName;
          if (launchFileDialog('o', filePathName, fileName))
          {
            loadPoints(curve1, curve2, filePathName);
            degreeA = curve1.getDegree();
            degreeB = curve2.getDegree();
            paramInfo.iPoints.clear();
          }
        }

        // launch a file dialog for saving points
        if (ImGui::Button("Save points to the file"))
        {
          curve1EditMode = false;
          curve2EditMode = false;
          std::string filePathName;
          std::string fileName;
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
          paramInfo.iPoints.clear();
        }
        if (ImGui::Button("Delete the last point from curve A"))
        {
          paramInfo.iPoints.clear();

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
          paramInfo.iPoints.clear();
        }

        //ImGui::Checkbox("Control Polygon for A", &curve1ControlPolygon);
        ImGui::Checkbox("Convex Hull for A", &curve1ConvexHull);

        if (ImGui::InputInt("degree A", &degreeA))
        {
          curve1EditMode = false;
          curve2EditMode = false;
          degreeA = std::clamp(degreeA, 1, 10);
          curve1.changeDegree(degreeA);
          paramInfo.iPoints.clear();
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
          paramInfo.iPoints.clear();
        }
        if (ImGui::Button("Delete the last point from curve B"))
        {
          paramInfo.iPoints.clear();

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
          paramInfo.iPoints.clear();
        }

        //ImGui::Checkbox("Control Polygon for B", &curve2ControlPolygon);
        ImGui::Checkbox("Convex Hull for B", &curve2ConvexHull);

        if (ImGui::InputInt("degree B", &degreeB))
        {
          curve1EditMode = false;
          curve2EditMode = false;
          degreeB = std::clamp(degreeB, 1, 10);
          curve2.changeDegree(degreeB);
          paramInfo.iPoints.clear();
        }

        if (curve2.getInterpolationPointSize() != 0 || curve2.getControlPointSize() != 0) ImGui::BeginDisabled();
        ImGui::Checkbox("Interpolation Mode for B", &curve2.interpolationMode);
        if (curve2.getInterpolationPointSize() != 0 || curve2.getControlPointSize() != 0) ImGui::EndDisabled();


        ImGui::Separator();
        ImGui::NewLine();

        ImGui::Checkbox("Decompose curves first", &paramInfo.decomposeFirst);


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
          curve1.drawCurve(window, sf::Color::Green, va1);
          curve2.drawCurve(window, sf::Color::Yellow, va2);
          window.display();

          paramInfo.iterationNum = 0;
          paramInfo.iPoints.clear();
          paramInfo.vNodeInfo.clear();

          if (Bspline::DEBUG)
          {
            Bspline::logFile.open("calc.log", std::ios::out);
            if (!Bspline::logFile.is_open())
            {
              std::println(stderr, "file open error");
              Bspline::DEBUG = false;
            }
          }

          findIntersection(curve1, curve2, paramInfo);

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

      curve1.drawCurve(window, sf::Color::Green, va1);

      /*if (curve1ControlPolygon)
      {
          curve1.drawControlPolygon(window);
      }*/

      if (curve1ConvexHull)
      {
        curve1.drawConvexHull(window, sf::Color::Cyan);
      }

      curve2.drawCurve(window, sf::Color::Yellow, va2);

      /*if (curve2ControlPolygon)
      {
          curve2.drawControlPolygon(window);
      }*/
      if (curve2ConvexHull)
      {
        curve2.drawConvexHull(window, sf::Color::Magenta);
      }

      for (auto& p : paramInfo.iPoints)
      {
        sf::CircleShape c{ 3 };
        c.setPosition({ static_cast<float>(p.x) - c.getRadius(), windowSize.y - static_cast<float>(p.y) - c.getRadius() });
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
    std::println(stderr, "{}", e.what());
    return 1;
  }
  catch (...)
  {
    std::println(stderr, "something wrong");
    return 1;
  }
}
