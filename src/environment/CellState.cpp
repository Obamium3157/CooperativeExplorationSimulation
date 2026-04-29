#include <iosfwd>

#include "CellState.h"

std::ostream& operator<<(std::ostream& os, const CellState& cellState)
{
    switch (cellState)
    {
    case CellState::Unknown: os << "."; break;
    case CellState::Free: os << " "; break;
    case CellState::Obstacle: os << "#"; break;
    case CellState::OccupiedByAgent: os << '@'; break;
    }

    return os;
}