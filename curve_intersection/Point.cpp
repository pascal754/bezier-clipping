// September 2, 2022
// by SM

module;

//#include <iostream>
//#include <cmath>

module Point;

import <iostream>;
import <cmath>;

const double Point::epsilon{0.5}; // chosen for screen pixel

std::ostream& operator<<(std::ostream& os, const Point& cp)
{
    os << "(" << cp.x << ", " << cp.y << ") ra = " << cp.angle; // << " " << col;
    return os;
}

//void Point::setColor(const Color& c)
//{
//	col = c;
//}

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
