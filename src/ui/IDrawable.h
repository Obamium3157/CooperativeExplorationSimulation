#ifndef COOPERATIVEEXPLORATIONSIMULATION_IDRAWABLE_H
#define COOPERATIVEEXPLORATIONSIMULATION_IDRAWABLE_H

#include <utility>
#include <vector>

#include "../environment/Point.h"

class Grid;

class IDrawable
{
public:
    virtual ~IDrawable() = default;

    virtual void Draw(const Grid& grid,
                      const std::vector<std::pair<Point, char>>& agentOverlay) = 0;

    virtual void OnSimulationFinished() {}
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_IDRAWABLE_H
