module;

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <format>
#include <cmath>

export module Auxilary;

import Point;

export void cleanIntersectionPoints(std::vector<Point>& iPoints)
{
    for (auto& pt : iPoints)
    {
        pt.x = std::round(pt.x);
        pt.y = std::round(pt.y);
    }

    auto comp = [](const Point& a, const Point& b) -> bool
    {
        if (a.x < b.x)
            return true;
        else if (a.x == b.x && a.y < b.y)
            return true;
        else
            return false;
    };

    std::sort(iPoints.begin(), iPoints.end(), comp);

    iPoints.erase(std::unique(iPoints.begin(), iPoints.end()), iPoints.end());
}

export void printResult(const int iterationNum, std::vector<Point>& iPoints, const bool DEBUG, std::ofstream& logFile)
{
    std::cout << '\t' << iterationNum << " decomposition(s)\n";

    std::cout << "the number of intersection before clean up: " << iPoints.size() << '\n';

    if (DEBUG)
    {
        logFile << '\t' << iterationNum << " decomposition(s)\n";
        logFile << "the number of intersection before clean up: " << iPoints.size() << '\n';

        for (int i{}; i < iPoints.size(); ++i)
        {
            logFile << std::format("***intersection point #{}: ", i + 1);
            logFile << iPoints[i] << '\n';
        }
    }

    if (iPoints.empty())
    {
        return;
    }

    cleanIntersectionPoints(iPoints);

    std::cout << "the number of intersection after clean up: " << iPoints.size() << '\n';

    if (iPoints.size() < 1'000)
    {
        for (int i{}; i < iPoints.size(); ++i)
        {
            std::cout << std::format("***intersection point #{}: ", i + 1);
            std::cout << iPoints[i] << '\n';
        }
    }

    std::cout << '\t' << iterationNum << " decomposition(s)\n";

    if (DEBUG)
    {
        logFile << "the number of intersection after clean up: " << iPoints.size() << '\n';

        for (int i{}; i < iPoints.size(); ++i)
        {
            logFile << std::format("***intersection point #{}: ", i + 1);
            logFile << iPoints[i] << '\n';
        }
    }
}