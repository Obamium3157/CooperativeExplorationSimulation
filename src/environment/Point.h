#ifndef COOPERATIVEEXPLORATIONSIMULATION_POINT_H
#define COOPERATIVEEXPLORATIONSIMULATION_POINT_H

#include <format>

using Coordinate = std::size_t;

struct Point
{
    Coordinate x = 0;
    Coordinate y = 0;

    operator std::string() const
    {
        return std::format("({}, {})", x, y);
    }
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_POINT_H
