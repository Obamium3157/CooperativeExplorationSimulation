#include <utility>
#include "Grid.h"

Grid::Grid(const Point& dimensions)
    : m_grid(dimensions.x, std::vector<Cell>(dimensions.y))
{
    for (std::size_t x = 0; x < dimensions.x; ++x)
    {
        for (std::size_t y = 0; y < dimensions.y; ++y)
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

Cell Grid::GetCell(const Point& point) const
{
    return m_grid[point.y][point.x];
}

void Grid::SetCell(const Point& point, const Cell& cell)
{
    m_grid[point.y][point.x] = cell;
}

Point Grid::GetDimensions() const
{
    return Point{m_grid.size(), m_grid[0].size()};
}
