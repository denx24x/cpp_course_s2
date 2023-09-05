#pragma once
#include <ostream>

class Point
{
public:
    Point(double x, double y);

    double x() const { return m_x; };
    double y() const { return m_y; };
    double distance(const Point &) const;

    bool operator<(const Point &) const;
    bool operator>(const Point &) const;
    bool operator<=(const Point &) const;
    bool operator>=(const Point &) const;
    bool operator==(const Point &) const;
    bool operator!=(const Point &) const;

    friend std::ostream & operator<<(std::ostream &, const Point &);

private:
    double m_x;
    double m_y;
};