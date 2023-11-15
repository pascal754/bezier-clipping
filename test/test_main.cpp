#include "pch.h"
#include <conio.h>

import Bspline;
import MakeCurve;

int main()
{
    Bspline c1{ 3 };
    Bspline c2{ 3 };
    c1.interpolationMode = false;
    c2.interpolationMode = false;

    ParamInfo pInfo{};

    std::vector<std::function<void(Bspline& curve1, Bspline& curve2)>> vf{ 
        make_curve01, make_curve02, make_curve03, make_curve04, make_curve05,
        make_curve06, make_curve07, make_curve08, make_curve09, make_curve10,
        make_curve11, make_curve12, make_curve13, make_curve14, make_curve15,
        make_curve16, make_curve17, make_curve18, make_curve19, make_curve20,
        make_curve21, make_curve22, make_curve23, make_curve24, make_curve25,
        make_curve26 };

    for (auto&& [index, make_crv] : std::views::enumerate(vf))
    {
        c1.clear();
        c2.clear();

        pInfo.iterationNum = 0;
        pInfo.iPoints.clear();
        pInfo.vNodeInfo.clear();

        make_crv(c1, c2);
        auto filename{ std::format("curves_{:02}.dat", index + 1) };
        savePoints(c1, c2, filename);

        /*findIntersection(c1, c2, pInfo);

        std::println("Press any key to continue ...");
        _getch();*/
    }
}