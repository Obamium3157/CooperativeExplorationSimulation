#include <utility>
#include "Grid.h"

Grid::Grid(const Point& dimensions)
    : m_grid(dimensions.y, std::vector<Cell>(dimensions.x))
{
    for (size_t y = 0; y < dimensions.y; ++y)
    {
        for (size_t x = 0; x < dimensions.x; ++x)
        {
            m_grid[y][x] = Cell{x, y, CellState::Unknown};
        }
    }
}

Grid::Grid(const GridMatrix& matrix)
    : m_grid(matrix)
{
}

Grid::Grid(GridMatrix&& matrix)
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
    if (m_grid.empty())
    {
        throw std::runtime_error("Grid matrix is empty");
    }
    return Point{m_grid[0].size(), m_grid.size()};
}
