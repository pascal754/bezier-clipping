// September 2, 2022
// by SM
module;

#include <iostream>
#include <format>
#include <cmath>

module Point;

const double Point::epsilon{ 0.5 }; // chosen for screen pixel

std::ostream& operator<<(std::ostream& os, const Point& cp)
{
    os << std::format("({}, {}) ra = {}", cp.x, cp.y, cp.angle);
    return os;
}

void Point::findAngleAround(const Point& pt)
{
    angle = std::atan2(y - pt.y, x - pt.x);
}

bool Point::hasSameCoord(const Point& pt) const
{
    if (x == pt.x && y == pt.y)
        return true;
    else
        return false;
}

bool Point::hasSameCoordWithTolerance(const Point& pt) const
{
    if (std::abs(x - pt.x) < epsilon && std::abs(y - pt.y) < epsilon)
        return true;
    else
        return false;
}

double Point::findDistance(const Point& pt) const
{
    double dx{ x - pt.x };
    double dy{ y - pt.y };
    return std::hypot(dx, dy);
}
