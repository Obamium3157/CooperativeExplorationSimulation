#ifndef COOPERATIVEEXPLORATIONSIMULATION_POINT_H
#define COOPERATIVEEXPLORATIONSIMULATION_POINT_H

#include <format>

struct Point
{
    size_t x = 0;
    size_t y = 0;

    bool operator==(const Point& other) const
    {
        return x == other.x && y == other.y;
    }

    operator std::string() const
    {
        return std::format("({}, {})", x, y);
    }
};

inline size_t ManhattanDistance(const Point& a, const Point& b) noexcept
{
    const size_t dx = a.x > b.x ? a.x - b.x : b.x - a.x;
    const size_t dy = a.y > b.y ? a.y - b.y : b.y - a.y;
    return dx + dy;
}

#endif //COOPERATIVEEXPLORATIONSIMULATION_POINT_H
