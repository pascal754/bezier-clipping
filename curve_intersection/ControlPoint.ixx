// September 2, 2022
// by SM

module;

#include <iostream>

export module Point;

export class Point {
public:
    Point() = default;
    Point(double xCoord, double yCoord) : x{ xCoord }, y{ yCoord } {}
    Point(const Point& pt) = default;
    friend std::ostream& operator<<(std::ostream& os, const Point& cp);
    //void setColor(Color c);
    void findAngleAround(const Point& pt);
    bool hasSameCoord(const Point& pt);
    double findDistance(const Point& pt);
    //void drawPoint(Graphics g, int h);
    double x{}, y{};
    double angle{};
private:
    static const double epsilon;
    // Color col;
};
