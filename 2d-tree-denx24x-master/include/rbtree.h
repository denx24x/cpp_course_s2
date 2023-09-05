#pragma once
#include "point.h"
#include "rect.h"

#include <optional>
#include <ostream>
#include <set>
#include <vector>

namespace rbtree {
class PointSet
{
public:
    class iterator
    {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = Point;
        using pointer = const value_type *;
        using reference = const value_type &;
        using iterator_category = std::forward_iterator_tag;

        iterator() = default;
        iterator(const std::vector<Point> & val);
        iterator(std::vector<Point> && val);
        reference operator*() const;
        pointer operator->() const;
        iterator & operator++();
        iterator operator++(int);
        friend bool operator==(const iterator &, const iterator &);
        friend bool operator!=(const iterator &, const iterator &);

    private:
        std::vector<Point> m_data;
    };

    PointSet(const std::string & filename = {});

    bool empty() const;
    std::size_t size() const;
    void put(const Point &);
    bool contains(const Point &) const;

    // second iterator points to an element out of range
    std::pair<iterator, iterator> range(const Rect &) const;
    iterator begin() const;
    iterator end() const;

    std::optional<Point> nearest(const Point &) const;
    // second iterator points to an element out of range
    std::pair<iterator, iterator> nearest(const Point & p, std::size_t k) const;

    friend std::ostream & operator<<(std::ostream &, const PointSet &);

private:
    std::set<Point> m_data;
};

} // namespace rbtree