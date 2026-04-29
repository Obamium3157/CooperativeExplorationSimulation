#ifndef COOPERATIVEEXPLORATIONSIMULATION_CELL_H
#define COOPERATIVEEXPLORATIONSIMULATION_CELL_H

#include "CellState.h"

#include "Point.h"

struct Cell
{
    Point position;
    CellState state = CellState::Unknown;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_CELL_H
