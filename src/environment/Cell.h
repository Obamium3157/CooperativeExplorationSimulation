#ifndef COOPERATIVEEXPLORATIONSIMULATION_CELL_H
#define COOPERATIVEEXPLORATIONSIMULATION_CELL_H

#include "CellState.h"

struct Cell
{
    unsigned int x = 0;
    unsigned int y = 0;
    CellState state = CellState::Unknown;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_CELL_H
