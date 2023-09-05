#include "rect.h"

#include <algorithm>
#include <cmath>

Rect::Rect(const Point & left_bottom, const Point & right_top)
    : m_left_bottom(left_bottom)
    , m_right_top(right_top)
{
}

double Rect::xmin() const
{
    return m_left_bottom.x();
}

double Rect::xmax() const
{
    return m_right_top.x();
}

double Rect::ymin() const
{
    return m_left_bottom.y();
}

double Rect::ymax() const
{
    return m_right_top.y();
}

double Rect::distance(const Point & p) const
{
    double dx = std::max({0.0, xmin() - p.x(), p.x() - xmax()});
    double dy = std::max({0.0, ymin() - p.y(), p.y() - ymax()});
    return sqrt(pow(dx, 2) + pow(dy, 2));
}

bool Rect::contains(const Point & p) const
{
    return xmin() <= p.x() && p.x() <= xmax() && ymin() <= p.y() && p.y() <= ymax();
}

bool Rect::intersects(const Rect & other) const
{
    return contains(other.m_right_top) ||
            contains({other.xmax(), other.ymin()}) ||
            contains({other.xmin(), other.ymax()}) ||
            contains(other.m_left_bottom) ||
            other.contains(m_left_bottom);
}
