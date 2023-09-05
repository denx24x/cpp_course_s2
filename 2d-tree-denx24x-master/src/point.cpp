#include "point.h"

#include <cmath>

Point::Point(double x, double y)
    : m_x(x)
    , m_y(y)
{
}

double Point::distance(const Point & other) const
{
    return sqrt(pow(m_x - other.m_x, 2) + pow(m_y - other.m_y, 2));
}

bool Point::operator==(const Point & other) const
{
    return std::abs(m_x - other.m_x) < std::numeric_limits<double>::epsilon() && std::abs(m_y - other.m_y) < std::numeric_limits<double>::epsilon();
}

bool Point::operator!=(const Point & other) const
{
    return !operator==(other);
}

bool Point::operator<(const Point & other) const
{
    return m_x < other.m_x || (std::abs(m_x - other.m_x) < std::numeric_limits<double>::epsilon() && m_y < other.m_y);
}

bool Point::operator<=(const Point & other) const
{
    return operator<(other) || operator==(other);
}

bool Point::operator>(const Point & other) const
{
    return !operator<=(other);
}

bool Point::operator>=(const Point & other) const
{
    return !operator<(other);
}

std::ostream & operator<<(std::ostream & stream, const Point & value)
{
    return stream << "(" << value.m_x << ", " << value.m_y << ")";
}