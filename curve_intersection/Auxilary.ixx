module;

#include <magic_enum/magic_enum.hpp>

export module Auxilary;

import Point;
import NodeInfo;
import std;

void cleanIntersectionPoints(std::vector<Point>& iPoints)
{
  for (auto& pt : iPoints)
  {
    pt.x = std::round(pt.x);
    pt.y = std::round(pt.y);
  }

  std::ranges::sort(iPoints);

  const auto ret{ std::ranges::unique(iPoints) };

  iPoints.erase(ret.begin(), ret.end());
}

export void printResult(const int iterationNum, std::vector<Point>& iPoints, const bool DEBUG, std::ofstream& logFile)
{
  std::println("\n\t* Calculation Summary *");

  std::println("\n\t{} iteration(s)", iterationNum);

  std::println("\nthe number of intersection points before clean up: {}", iPoints.size());

  namespace rv = std::ranges::views;

  auto writePoints = [&](std::ostream& os) {
    for (auto&& [index, pt] : iPoints | rv::enumerate | rv::as_const)
    {
      std::println(os, "***intersection point #{}: ({}, {})", index + 1, pt.x, pt.y);
    }
    };

  if (DEBUG)
  {
    std::println(logFile, "\n--- calculation results ---");
    std::println(logFile, "\n\t{} iteration(s)", iterationNum);
    std::println(logFile, "the number of intersection points before clean up: {}", iPoints.size());

    std::ranges::sort(iPoints);

    writePoints(logFile);
  }

  if (iPoints.empty())
  {
    return;
  }

  cleanIntersectionPoints(iPoints);

  std::println("\nthe number of intersection points after clean up: {}", iPoints.size());

  if (DEBUG)
  {
    std::println(logFile, "\nthe number of intersection after clean up: {}", iPoints.size());

    writePoints(logFile);

    std::println("\nDebug info written to 'calc.log'.");
  }
}

export void writeNodeInfo(const std::vector<NodeInfo>& vNodeInfo)
{
  std::print("\nWriting iteration info to 'node_info.log' ... ");

  std::ofstream file("node_info.log");
  std::println(file, "{:>10}{:>10}{:>5}  {:^23}  {:^23}  {:>5}  {:^23}  {:^23}  {:5}  {:^15}  {:>5}",
    "Iteration",
    "From",
    "ID",
    "[u1",
    "u2]",
    "ID",
    "[u1",
    "u2]",
    "First",
    "Info",
    "Depth");

  for (const auto& x : vNodeInfo)
  {
    if (x.curveA.id == 0)
    {
      std::println(file, "{:10}{:10}{:5}  {:<23}  {:<23}  {:5}  {:<23}  {:<23}  {:^5}  {:15}  {:5}",
        x.iterationNum,
        x.parentNum,
        x.curveA.id,
        x.curveA.u1,
        x.curveA.u2,
        x.curveB.id,
        x.curveB.u1,
        x.curveB.u2,
        x.curveA.id,
        magic_enum::enum_name(x.rInfo),
        x.depth);
    }
    else
    {
      std::println(file, "{:10}{:10}{:5}  {:<23}  {:<23}  {:5}  {:<23}  {:<23}  {:^5}  {:15}  {:5}",
        x.iterationNum,
        x.parentNum,
        x.curveB.id,
        x.curveB.u1,
        x.curveB.u2,
        x.curveA.id,
        x.curveA.u1,
        x.curveA.u2,
        x.curveA.id,
        magic_enum::enum_name(x.rInfo),
        x.depth);
    }
  }

  std::println("Done.");
}
