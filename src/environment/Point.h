#ifndef COOPERATIVEEXPLORATIONSIMULATION_POINT_H
#define COOPERATIVEEXPLORATIONSIMULATION_POINT_H

#include <format>

struct Point
{
    std::size_t x = 0;
    std::size_t y = 0;

    operator std::string() const
    {
        return std::format("({}, {})", x, y);
    }
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_POINT_H
