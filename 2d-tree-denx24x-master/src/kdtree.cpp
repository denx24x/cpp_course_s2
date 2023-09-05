#include "kdtree.h"

#include <algorithm>
#include <fstream>
#include <queue>

namespace kdtree {
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
}

void Node::put(const Point & val)
{
    UpdateRegion(val);
    if (compare(val) >= 0) {
        if (left) {
            left->put(val);
        }
        else {
            left = std::make_unique<Node>(val, !cmp_x);
        }
    }
    else {
        if (right) {
            right->put(val);
        }
        else {
            right = std::make_unique<Node>(val, !cmp_x);
        }
    }
}

void PointSet::put(const Point & val)
{
    if (!contains(val)) {
        m_size++;
        if (root) {
            root->put(val);
        }
        else {
            root = std::make_unique<Node>(val, true);
        }
    }
}

bool PointSet::contains(const Point & val) const
{
    if (!root) {
        return false;
    }
    return root->contains(val);
}

bool Node::contains(const Point & val) const
{
    if (val == value) {
        return true;
    }

    if (left && compare(val) >= 0) {
        return left->contains(val);
    }
    else if (right && compare(val) < 0) {
        return right->contains(val);
    }

    return false;
}

std::vector<Point> Node::FindInRect(const Rect & val) const
{
    std::vector<Point> ans;
    if (val.contains(value)) {
        ans.push_back(value);
    }
    if (left && left->region.intersects(val)) {
        auto bf = left->FindInRect(val);
        std::move(bf.begin(), bf.end(), std::back_inserter(ans));
    }
    if (right && right->region.intersects(val)) {
        auto bf = right->FindInRect(val);
        std::move(bf.begin(), bf.end(), std::back_inserter(ans));
    }
    return ans;
}

std::optional<Point> PointSet::nearest(const Point & val) const
{
    if (!root) {
        return {};
    }
    std::priority_queue<std::pair<double, Point>> pq;
    root->nearest(val, 1, pq);
    if (pq.empty()) {
        return {};
    }
    return pq.top().second;
}

void Node::nearest(const Point & val, std::size_t k, std::priority_queue<std::pair<double, Point>> & pq) const
{
    double dst = val.distance(value);
    if (pq.size() < k || dst < pq.top().first) {
        pq.push({dst, value});
        if (pq.size() > k) {
            pq.pop();
        }
    }
    if (left && right) {
        if (left->region.distance(val) <= right->region.distance(val)) {
            left->nearest(val, k, pq);
            if (pq.size() < k || right->region.distance(val) <= pq.top().first) {
                right->nearest(val, k, pq);
            }
            return;
        }
        right->nearest(val, k, pq);
        if (pq.size() < k || left->region.distance(val) <= pq.top().first) {
            left->nearest(val, k, pq);
        }
        return;
    }
    if (left && (pq.size() < k || left->region.distance(val) <= pq.top().first)) {
        return left->nearest(val, k, pq);
    }
    if (right && (pq.size() < k || right->region.distance(val) <= pq.top().first)) {
        return right->nearest(val, k, pq);
    }
}

std::pair<PointSet::iterator, PointSet::iterator> PointSet::nearest(const Point & val, std::size_t k) const
{
    if (k == 0) {
        return {PointSet::iterator(), PointSet::iterator()};
    }
    std::vector<Point> answer;
    std::priority_queue<std::pair<double, Point>> pq;
    if (root) {
        root->nearest(val, k, pq);
    }
    while (!pq.empty()) {
        answer.push_back(pq.top().second);
        pq.pop();
    }
    reverse(answer.begin(), answer.end());
    return {PointSet::iterator(std::move(answer)), PointSet::iterator()};
}

Node::Node(const Node & other)
    : value(other.value)
    , left(other.left ? std::make_unique<Node>(*other.left) : nullptr)
    , right(other.right ? std::make_unique<Node>(*other.right) : nullptr)
    , cmp_x(other.cmp_x)
    , region(other.region)
{
}

PointSet::PointSet(const PointSet & other)
    : root(other.root ? std::make_unique<Node>(*other.root) : nullptr)
{
}

bool PointSet::empty() const
{
    return !root;
}

std::size_t PointSet::size() const
{
    return m_size;
}

std::vector<Point> Node::GetDfs() const
{
    std::vector<Point> ans;
    if (left) {
        auto val = left->GetDfs();
        std::move(val.begin(), val.end(), std::back_inserter(ans));
    }
    if (right) {
        auto val = right->GetDfs();
        std::move(val.begin(), val.end(), std::back_inserter(ans));
    }
    ans.push_back(value);
    return ans;
}

void Node::UpdateRegion(const Point & val)
{
    Point left_bottom = {std::min(val.x(), region.xmin()), std::min(val.y(), region.ymin())};
    Point right_top = {std::max(val.x(), region.xmax()), std::max(val.y(), region.ymax())};
    region = Rect(left_bottom, right_top);
}

int Node::compare(const Point & other) const
{
    double left = cmp_x ? value.x() : value.y();
    double right = cmp_x ? other.x() : other.y();
    return left < right ? -1 : (left > right ? 1 : 0);
}

PointSet::iterator PointSet::begin() const
{
    if (!root) {
        return PointSet::iterator();
    }
    return PointSet::iterator(root->GetDfs());
}

PointSet::iterator PointSet::end() const
{
    return PointSet::iterator();
}

PointSet::iterator::iterator(const std::vector<Point> & val)
    : m_data(val)
{
}
PointSet::iterator::iterator(std::vector<Point> && val)
    : m_data(std::move(val))
{
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
    if (!root) {
        return {PointSet::iterator(), PointSet::iterator()};
    }
    return {PointSet::iterator(root->FindInRect(val)), PointSet::iterator()};
}

std::ostream & operator<<(std::ostream & stream, const PointSet &)
{
    return stream << "";
}
} // namespace kdtree