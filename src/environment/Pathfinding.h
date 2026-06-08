#ifndef COOPERATIVEEXPLORATIONSIMULATION_PATHFINDING_H
#define COOPERATIVEEXPLORATIONSIMULATION_PATHFINDING_H

#include <optional>
#include <vector>

#include "Grid.h"
#include "Point.h"

namespace Pathfinding
{
    /**
     * @return std::nullopt if path does not exists
     */
    std::optional<std::vector<Point>> FindPath(const Point& from, const Point& to,
                                               const Grid& beliefMap);

    /**
     * @return std::nullopt if path does not exists
     */
    std::optional<size_t> FindPathLength(const Point& from, const Point& to,
                                         const Grid& beliefMap);
}

#endif //COOPERATIVEEXPLORATIONSIMULATION_PATHFINDING_H
