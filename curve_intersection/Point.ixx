// September 2, 2022
// by SM

export module Point;

import <iostream>;

export class Point {
public:
    static const double epsilon;
    Point() = default;
    Point(double xCoord, double yCoord) : x{ xCoord }, y{ yCoord } {}
    Point(const Point& pt) = default;
    friend std::ostream& operator<<(std::ostream& os, const Point& cp);
    //void setColor(Color c);
    void findAngleAround(const Point& pt);
    bool hasSameCoord(const Point& pt) const;
    bool hasSameCoordWithTolerance(const Point& pt) const;
    double findDistance(const Point& pt) const;
    double x{}, y{};
    double angle{};
private:
    // Color col;
};
