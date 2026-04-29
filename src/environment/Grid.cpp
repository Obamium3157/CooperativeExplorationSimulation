#include <utility>
#include "Grid.h"

Grid::Grid(const unsigned int width, const unsigned int height)
    : m_grid(width, std::vector<Cell>(height))
{
    for (unsigned int x = 0; x < width; ++x)
    {
        for (unsigned int y = 0; y < height; ++y)
        {
            m_grid[y][x] = Cell{x, y, CellState::Unknown};
        }
    }
}

Grid::Grid(GridMatrix matrix)
    : m_grid(std::move(matrix))
{
}

const GridMatrix& Grid::GetGridMatrix() const
{
    return m_grid;
}

Cell Grid::GetCell(const unsigned int x, const unsigned int y) const
{
    return m_grid[y][x];
}

void Grid::SetCell(const unsigned int x, const unsigned int y, const Cell& cell)
{
    m_grid[y][x] = cell;
}

unsigned int Grid::GetWidth() const
{
    return m_grid.size();
}

unsigned int Grid::GetHeight() const
{
    return m_grid[0].size();
}
