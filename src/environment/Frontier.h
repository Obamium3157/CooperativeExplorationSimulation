#ifndef COOPERATIVEEXPLORATIONSIMULATION_FRONTIER_H
#define COOPERATIVEEXPLORATIONSIMULATION_FRONTIER_H

#include <vector>

#include "Grid.h"
#include "Point.h"

namespace Frontier
{
    std::vector<Point> OrthogonalNeighbors(const Point& point, const Point& dimensions);
    bool IsFrontierCell(const Point& point, const Grid& beliefMap);
    std::vector<Point> ComputeFrontiers(const Grid& beliefMap);
}

#endif //COOPERATIVEEXPLORATIONSIMULATION_FRONTIER_H
