#include "ConsoleDrawer.h"
#include "../environment/Grid.h"

ConsoleDrawer::ConsoleDrawer(std::ostream& out)
    : m_out(out)
{
}

void ConsoleDrawer::Draw(const Grid& grid)
{
    const unsigned int width = grid.GetWidth();
    const unsigned int height = grid.GetHeight();

    DrawHorizontalBorder(width);

    for (unsigned int y = 0; y < height; ++y)
    {
        m_out << '|';
        for (unsigned int x = 0; x < width; ++x)
        {
            DrawCell(grid.GetCell(x, y));
        }
        m_out << "|\n";
    }

    DrawHorizontalBorder(width);
}

void ConsoleDrawer::DrawHorizontalBorder(const unsigned int width) const
{
    m_out << '+';
    for (unsigned int i = 0; i < width; ++i)
    {
        m_out << '-';
    }
    m_out << "+\n";
}

void ConsoleDrawer::DrawCell(const Cell& cell) const
{
    m_out << cell.state;
}
