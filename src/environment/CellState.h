#ifndef COOPERATIVEEXPLORATIONSIMULATION_CELLSTATE_H
#define COOPERATIVEEXPLORATIONSIMULATION_CELLSTATE_H

#include <ostream>

enum class CellState
{
    Unknown,
    Free,
    Obstacle,
    OccupiedByAgent,
};

std::ostream& operator<<(std::ostream& os, const CellState& cellState);

#endif //COOPERATIVEEXPLORATIONSIMULATION_CELLSTATE_H
