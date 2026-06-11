#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>
#include "ConsoleDrawer.h"
#include "DrawableCharacters.h"
#include "../environment/Grid.h"

namespace
{
    void ClearConsole(std::ostream& m_out)
    {
        m_out << "\033[2J\033[1;1H";
    }
}

ConsoleDrawer::ConsoleDrawer(std::ostream& out, const unsigned int sleepTime)
    : m_out(out)
    , m_sleepTime(sleepTime)
{
}

void ConsoleDrawer::Draw(const Grid& grid,
                         const std::vector<std::pair<Point, char>>& agentOverlay)
{
    ClearConsole(m_out);

    const auto [width, height] = grid.GetDimensions();

    DrawHorizontalBorder(width);

    for (unsigned int y = 0; y < height; ++y)
    {
        m_out << DrawableCharacter::GridBorderVertical;
        for (unsigned int x = 0; x < width; ++x)
        {
            const Point currentPoint{x, y};
            const auto agentIt = std::find_if(
                agentOverlay.begin(), agentOverlay.end(),
                [&currentPoint](const auto& entry) { return entry.first == currentPoint; });

            if (agentIt != agentOverlay.end())
            {
                m_out << agentIt->second;
            }
            else
            {
                DrawCell(grid.GetCell(currentPoint));
            }
        }
        m_out << DrawableCharacter::GridBorderVertical << '\n';
    }

    DrawHorizontalBorder(width);
    m_out << '\n';

    std::this_thread::sleep_for(std::chrono::milliseconds(m_sleepTime));
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
