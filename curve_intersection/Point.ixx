// September 2, 2022
// by SM

module;

#include "pch.h"

export module Point;

export class Point {
public:
    static const double epsilon;
    Point() = default;
    Point(double xCoord, double yCoord) : x{ xCoord }, y{ yCoord } {}
    friend std::ostream& operator<<(std::ostream& os, const Point& cp);
    void findAngleAround(const Point& pt);
    bool hasSameCoord(const Point& pt) const;
    bool hasSameCoordWithTolerance(const Point& pt) const;
    double findDistance(const Point& pt) const;
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    auto operator<=>(const Point& other) const
    {
        // perform a three-way comparison between the
        // x's. If that result != 0 (that is, the x's
        // differ), then that's the result of the
        // overall comparison
        if (auto c = x <=> other.x; c != 0) return c;

        // otherwise, proceed to comparing the next
        // pair of elements
        return y <=> other.y;
    }
    double x{};
    double y{};
    double angle{};
};

template <>
struct std::formatter<Point> : std::formatter<std::string>
{
    auto format(Point p, format_context& ctx) const
    {
        return formatter<string>::format(
            std::format("({}, {}) ra = {}", p.x, p.y, p.angle), ctx);
    }
};
