#pragma once
#include "point.h"
#include "rect.h"

#include <memory>
#include <optional>
#include <ostream>
#include <queue>
#include <vector>

namespace kdtree {

struct Node
{
    Point value;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    bool cmp_x;
    Rect region;

    void UpdateRegion(const Point & val);
    int compare(const Point & other) const;
    std::vector<Point> FindInRect(const Rect & val) const;
    std::vector<Point> GetDfs() const;
    bool contains(const Point &) const;
    void put(const Point &);
    void nearest(const Point &, std::size_t k, std::priority_queue<std::pair<double, Point>> & pq) const;

    Node(const Node & other);
    Node(const Point & val, bool flag)
        : value(val)
        , cmp_x(flag)
        , region(val, val){};
};
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
        iterator(std::vector<Point> && val);
        iterator(const std::vector<Point> & val);
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
    PointSet(const PointSet & other);

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
    std::unique_ptr<Node> root;
    std::size_t m_size = 0;
};

} // namespace kdtree