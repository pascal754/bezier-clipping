module;

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <format>
#include <cmath>
#include <ranges>

export module Auxilary;

import Point;

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
    std::cout << "\n--- calculation results ---\n";

    std::cout << "\n\t" << iterationNum << " iteration(s)\n";

    std::cout << "the number of intersection before clean up: " << iPoints.size() << '\n';

    using namespace std::views;

    auto writePoints = [&](std::ostream& os) {
        for (auto [index, pt] : iPoints | enumerate | as_const)
        {
            os << std::format("***intersection point #{}: ", index + 1);
            os << '(' << pt.x << ", " << pt.y << ")\n";
        }
        };

    if (iPoints.size() <= 1'000)
    {
        writePoints(std::cout);
    }

    if (DEBUG)
    {
        logFile << "\n--- calculation results ---\n";
        logFile << "\n\t" << iterationNum << " iteration(s)\n";
        logFile << "the number of intersection before clean up: " << iPoints.size() << '\n';

        std::sort(iPoints.begin(), iPoints.end());

        writePoints(logFile);
    }

    if (iPoints.empty())
    {
        return;
    }

    cleanIntersectionPoints(iPoints);

    std::cout << "\nthe number of intersection after clean up: " << iPoints.size() << '\n';

    if (iPoints.size() <= 1'000)
    {
        writePoints(std::cout);
    }

    std::cout << '\t' << iterationNum << " iteration(s)\n";

    if (DEBUG)
    {
        logFile << "\nthe number of intersection after clean up: " << iPoints.size() << '\n';

        writePoints(logFile);
    }
}