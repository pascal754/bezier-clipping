// test for global interpolation
import Point;
import Bspline;

auto main() -> int
{
    Bspline curve1{ 3 };
    curve1.addInterpolationPoint(Point{ 0, 0 });
    curve1.addInterpolationPoint(Point{ 3, 4 });
    curve1.addInterpolationPoint(Point{ -1, 4 });
    curve1.addInterpolationPoint(Point{ -4, 0 });
    curve1.addInterpolationPoint(Point{ -4, 0 });
    curve1.addInterpolationPoint(Point{ -4, -3 });

    curve1.globalCurveInterpolation();
}