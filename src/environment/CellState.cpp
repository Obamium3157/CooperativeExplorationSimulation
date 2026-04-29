#include <iosfwd>

#include "CellState.h"

#include "../ui/DrawableCharacters.h"

std::ostream& operator<<(std::ostream& os, const CellState& cellState)
{
    switch (cellState)
    {
    case CellState::Unknown: os << DrawableCharacter::Unknown; break;
    case CellState::Free: os << DrawableCharacter::Free; break;
    case CellState::Obstacle: os << DrawableCharacter::Obstacle; break;
    case CellState::OccupiedByAgent: os << DrawableCharacter::Agent; break;
    }

    return os;
}
