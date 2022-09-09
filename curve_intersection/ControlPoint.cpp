// filename: Point.java
// October 1997
// 11 Oct, 1997
module;

#include <iostream>
#include <cmath>

module Point;

const double Point::epsilon{1e-9};

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

bool Point::hasSameCoord(const Point& pt)
{
	if (x == pt.x && y == pt.y) // (std::abs(x - pt.x) < epsilon && std::abs(y - pt.y) < epsilon)
		return true;
	else
		return false;
}

double Point::findDistance(const Point& pt) {
    double dx{ x - pt.x };
    double dy{ y - pt.y };
	return std::hypot(dx, dy);
}

//void Point::drawPoint(Graphics g, int h) {
//	g.fillOval((int)x - 2, h - (int)y - 2, 4, 4);
//}
