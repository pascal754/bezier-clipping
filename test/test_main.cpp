#include "pch.h"

import Bspline;
import MakeCurve;
import std;

int main()
{
    try
    {
        find_intersection_batch();
    }
    catch (const std::exception& e)
    {
        std::println(stderr, "{}", e.what());
    }
    catch (...)
    {
        std::println(stderr, "other error");
    }
}