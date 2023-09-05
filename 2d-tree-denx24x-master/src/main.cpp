#include "primitives.h"

#include <iostream>

int main()
{
    kdtree::PointSet p("");
    p.put(Point(.7, .2));
    p.put(Point(.5, .4));
    p.put(Point(.2, .3));
    p.put(Point(.4, .7));
    p.put(Point(.9, .6));

    for (auto i = p.begin(); i != p.end(); i++) {
        std::cout << *i << "\n";
    }
}
