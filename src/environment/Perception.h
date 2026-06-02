#ifndef COOPERATIVEEXPLORATIONSIMULATION_PERCEPTION_H
#define COOPERATIVEEXPLORATIONSIMULATION_PERCEPTION_H

#include "Grid.h"
#include "Point.h"
#include "Cell.h"
#include <vector>

namespace Perception
{
    bool HasLineOfSight(const Point& from, const Point& to, const Grid& grid);
    std::vector<Cell> GetPerceivedCells(const Point& position, double radius, const Grid& realGrid);
}

#endif //COOPERATIVEEXPLORATIONSIMULATION_PERCEPTION_H
