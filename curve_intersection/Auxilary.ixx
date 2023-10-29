module;

#include "pch.h"

export module Auxilary;

import Point;
import NodeInfo;

export void cleanIntersectionPoints(std::vector<Point>& iPoints)
{
    for (auto& pt : iPoints)
    {
        pt.x = std::round(pt.x);
        pt.y = std::round(pt.y);
    }

    std::sort(iPoints.begin(), iPoints.end());

    iPoints.erase(std::unique(iPoints.begin(), iPoints.end()), iPoints.end());
}

export void printResult(const int iterationNum, std::vector<Point>& iPoints, const bool DEBUG, std::ofstream& logFile)
{
    std::println("\n--- calculation results ---");

    std::println("\n\t{} iteration(s)", iterationNum);

    std::println("the number of intersection before clean up: {}", iPoints.size());

    using namespace std::views;

    auto writePoints = [&](std::ostream& os) {
        for (auto [index, pt] : iPoints | enumerate | as_const)
        {
            std::println(os, "***intersection point #{}: ({}, {})", index + 1, pt.x, pt.y);
        }
        };

    if (iPoints.size() <= 1'000)
    {
        writePoints(std::cout);
    }

    if (DEBUG)
    {
        std::println(logFile, "\n--- calculation results ---");
        std::println(logFile, "\n\t{} iteration(s)", iterationNum);
        std::println(logFile, "the number of intersection before clean up: {}", iPoints.size());

        std::sort(iPoints.begin(), iPoints.end());

        writePoints(logFile);
    }

    if (iPoints.empty())
    {
        return;
    }

    cleanIntersectionPoints(iPoints);

    std::println("\nthe number of intersection after clean up: {}", iPoints.size());

    if (iPoints.size() <= 1'000)
    {
        writePoints(std::cout);
    }

    std::println("\t{} iteration(s)", iterationNum);

    if (DEBUG)
    {
        std::println(logFile, "\nthe number of intersection after clean up: {}", iPoints.size());

        writePoints(logFile);
    }
}

export void writeNodeInfo(const std::vector<NodeInfo>& vNodeInfo)
{
    std::ofstream file("node_info.log");
    std::println(file, "{:>10}{:>10}{:>5}  {:^23}  {:^23}  {:>5}  {:^23}  {:^23}  {:^15}  {:>5}",
        "Iteration",
        "From",
        "ID",
        "[u1",
        "u2]",
        "ID",
        "[u1",
        "u2]",
        "Info",
        "Depth");
    for (const auto& x : vNodeInfo)
    {

        std::println(file, "{:10}{:10}{:5}  {:<23}  {:<23}  {:5}  {:<23}  {:<23}  {:15}  {:5}",
            x.iterationNum,
            x.parentNum,
            x.curveA.id,
            x.curveA.u1,
            x.curveA.u2,
            x.curveB.id,
            x.curveB.u1,
            x.curveB.u2,
            magic_enum::enum_name(x.rInfo),
            x.depth);
    }
}