// September 2, 2022
// by SM

export module Point;

import std;

export class Point
{
public:
  Point() = default;
  Point(double xCoord, double yCoord) : x{ xCoord }, y{ yCoord } {}
  friend std::ostream& operator<<(std::ostream& os, const Point& cp);
  double findAngleAround(const Point& pt);
  bool hasSameCoordWithTolerance(const Point& pt) const;
  double findDistance(const Point& pt) const;
  bool operator==(const Point& other) const { return x == other.x && y == other.y; }
  auto operator<=>(const Point& other) const
  {
    // perform a three-way comparison between the
    // x's. If that result != 0 (that is, the x's
    // differ), then that's the result of the
    // overall comparison
    if (auto c = x <=> other.x; c != 0)
    {
      return c;
    }

    // otherwise, proceed to comparing the next
    // pair of elements
    return y <=> other.y;
  }

  double x{};
  double y{};

private:
  constexpr static double epsilon{ 0.1 }; // chosen for a screen pixel unit
};

export struct CHPoint //convex hull points
{
  CHPoint(double x, double y, double a) : p{ x, y }, angle{ a } {}
  Point p;
  double angle{};
};

// raises errors in __msvc_formatter.hpp using Microsoft Visual Studio Community 2022 (64-bit) - Preview
// Version 17.12.0 Preview 2.1

//template <>
//struct std::formatter<Point> : std::formatter<std::string>
//{
//    auto format(const Point& p, std::format_context& ctx) const
//    {
//        return std::formatter<string>::format(
//            std::format("({}, {})", p.x, p.y), ctx);
//    }
//};

// a workaround to avoid the errors above
template <>
struct std::formatter<Point>
{
  constexpr auto parse(std::format_parse_context& ctx)
  {
    return ctx.begin();
  }

  auto format(const Point& p, std::format_context& ctx) const
  {
    return std::format_to(ctx.out(), "({}, {})", p.x, p.y);
  }
};
