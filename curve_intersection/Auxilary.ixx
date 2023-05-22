export module Auxilary;

import <iostream>;
import <vector>;
import <algorithm>;
import <fstream>;
import <format>;
import <cmath>;
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
    std::cout << "\n\t" << iterationNum << " iteration(s)\n";

    std::cout << "the number of intersection before clean up: " << iPoints.size() << '\n';

    if (DEBUG)
    {
        logFile << "\n\t" << iterationNum << " iteration(s)\n";
        logFile << "the number of intersection before clean up: " << iPoints.size() << '\n';

        std::sort(iPoints.begin(), iPoints.end());

        for (int i{ 1 }; const auto & pt : iPoints)
        {
            logFile << std::format("***intersection point #{}: ", i);
            logFile << pt << '\n';
            ++i;
        }
    }

    if (iPoints.empty())
    {
        return;
    }

    cleanIntersectionPoints(iPoints);

    std::cout << "\nthe number of intersection after clean up: " << iPoints.size() << '\n';

    if (iPoints.size() < 1'000)
    {
        for (int i{ 1 }; const auto & pt : iPoints)
        {
            std::cout << std::format("***intersection point #{}: ", i);
            std::cout << pt << '\n';
            ++i;
        }
    }

    std::cout << '\t' << iterationNum << " iteration(s)\n";

    if (DEBUG)
    {
        logFile << "\nthe number of intersection after clean up: " << iPoints.size() << '\n';

        for (int i{ 1 }; const auto & pt: iPoints)
        {
            logFile << std::format("***intersection point #{}: ", i);
            logFile << pt << '\n';
            ++i;
        }
    }
}