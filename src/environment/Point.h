#ifndef COOPERATIVEEXPLORATIONSIMULATION_POINT_H
#define COOPERATIVEEXPLORATIONSIMULATION_POINT_H

#include <format>

struct Point
{
    size_t x = 0;
    size_t y = 0;

    operator std::string() const
    {
        return std::format("({}, {})", x, y);
    }
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_POINT_H
