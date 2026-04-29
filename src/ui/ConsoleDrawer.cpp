#include <iostream>
#include "ConsoleDrawer.h"
#include "DrawableCharacters.h"
#include "../environment/Grid.h"

ConsoleDrawer::ConsoleDrawer(std::ostream& out)
    : m_out(out)
{
}

void ConsoleDrawer::Draw(const Grid& grid)
{
    const auto [width, height] = grid.GetDimensions();

    DrawHorizontalBorder(width);

    for (unsigned int y = 0; y < height; ++y)
    {
        m_out << DrawableCharacter::GridBorderVertical;
        for (unsigned int x = 0; x < width; ++x)
        {
            DrawCell(grid.GetCell(Point{x, y}));
        }
        m_out << DrawableCharacter::GridBorderVertical << '\n';
    }

    DrawHorizontalBorder(width);
}

void ConsoleDrawer::DrawHorizontalBorder(const unsigned int width) const
{
    m_out << DrawableCharacter::GridCorner;
    for (unsigned int i = 0; i < width; ++i)
    {
        m_out << DrawableCharacter::GridBorderHorizontal;
    }
    m_out << DrawableCharacter::GridCorner << '\n';
}

void ConsoleDrawer::DrawCell(const Cell& cell) const
{
    m_out << cell.state;
}

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
