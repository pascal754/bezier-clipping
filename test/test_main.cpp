#include "pch.h"

import Bspline;
import MakeCurve;

int main()
{
    try
    {
        decompose_test();
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