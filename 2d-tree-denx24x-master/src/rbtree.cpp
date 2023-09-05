#include "rbtree.h"

#include <algorithm>
#include <fstream>
#include <queue>

namespace rbtree {
PointSet::PointSet(const std::string & filename)
{
    std::ifstream input(filename);
    if (input.fail()) {
        return;
    }
    double first, second;
    while (input >> first && input >> second) {
        put({first, second});
    }
    input.close();
}

void PointSet::put(const Point & val)
{
    m_data.insert(val);
}

bool PointSet::contains(const Point & val) const
{
    return m_data.find(val) != m_data.end();
}

std::optional<Point> PointSet::nearest(const Point & val) const
{
    auto ans = std::min_element(m_data.begin(), m_data.end(), [&val](const Point & a, const Point & b) { return val.distance(a) < val.distance(b); });
    if (ans == m_data.end()) {
        return {};
    }
    return *ans;
}

std::pair<PointSet::iterator, PointSet::iterator> PointSet::nearest(const Point & val, std::size_t k) const
{
    std::vector<Point> ans;
    std::priority_queue<std::pair<double, Point>> pq;
    for (const auto i : m_data) {
        double dst = i.distance(val);
        if (pq.size() < k || (!pq.empty() && dst < pq.top().first)) {
            pq.push({dst, i});
            if (pq.size() > k) {
                pq.pop();
            }
        }
    }
    while (!pq.empty()) {
        ans.push_back(pq.top().second);
        pq.pop();
    }
    return {PointSet::iterator(std::move(ans)), PointSet::iterator()};
}

bool PointSet::empty() const
{
    return m_data.empty();
}

std::size_t PointSet::size() const
{
    return m_data.size();
}

PointSet::iterator::iterator(const std::vector<Point> & val)
    : m_data(val)
{
}
PointSet::iterator::iterator(std::vector<Point> && val)
    : m_data(std::move(val))
{
}

PointSet::iterator PointSet::begin() const
{
    return PointSet::iterator(std::vector(m_data.begin(), m_data.end()));
}

PointSet::iterator PointSet::end() const
{
    return PointSet::iterator(std::vector<Point>());
}

PointSet::iterator::reference PointSet::iterator::operator*() const
{
    return m_data.back();
}

PointSet::iterator::pointer PointSet::iterator::operator->() const
{
    return &m_data.back();
}

PointSet::iterator & PointSet::iterator::operator++()
{
    m_data.pop_back();
    return *this;
}

PointSet::iterator PointSet::iterator::operator++(int)
{
    auto tmp = *this;
    operator++();
    return tmp;
}

bool operator==(const PointSet::iterator & left, const PointSet::iterator & right)
{
    if (left.m_data.empty() || right.m_data.empty()) {
        return left.m_data.empty() == right.m_data.empty();
    }
    return *left == *right;
}

bool operator!=(const PointSet::iterator & left, const PointSet::iterator & right)
{
    return !(left == right);
}

std::pair<PointSet::iterator, PointSet::iterator> PointSet::range(const Rect & val) const
{
    std::vector<Point> value;
    auto end = m_data.upper_bound(val.m_right_top);
    for (auto i = m_data.lower_bound(val.m_left_bottom); i != end; i++) {
        if (val.contains(*i)) {
            value.push_back(*i);
        }
    }
    return {PointSet::iterator(std::move(value)), PointSet::iterator(std::vector<Point>())};
}

std::ostream & operator<<(std::ostream & stream, const PointSet &)
{
    return stream << "";
}
} // namespace rbtree