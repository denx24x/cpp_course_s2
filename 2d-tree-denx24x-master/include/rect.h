#pragma once
#include "point.h"

class Rect
{
public:
    Rect(const Point & left_bottom, const Point & right_top);

    double xmin() const;
    double ymin() const;
    double xmax() const;
    double ymax() const;
    double distance(const Point & p) const;

    bool contains(const Point & p) const;
    bool intersects(const Rect & other) const;

    Point m_left_bottom;
    Point m_right_top;
};