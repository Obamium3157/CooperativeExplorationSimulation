#include <iostream>

#include "Grid.h"
#include "../ui/ConsoleDrawer.h"
#include "../ui/GUIDrawer.h"

Grid::Grid(const unsigned int width, const unsigned int height, const DrawableVariant variant)
    : m_grid(width, std::vector<Cell>(height))
    , m_drawable(MakeDrawable(variant))
{
    for (unsigned int x = 0; x < width; ++x)
    {
        for (unsigned int y = 0; y < height; ++y)
        {
            m_grid[y][x] = Cell{x, y, CellState::Unknown};
        }
    }
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

void Grid::Render() const
{
    m_drawable->Draw(*this);
}

std::unique_ptr<IDrawable> Grid::MakeDrawable(const DrawableVariant variant)
{
    switch (variant)
    {
    case DrawableVariant::Console: return std::make_unique<ConsoleDrawer>(std::cout);
    case DrawableVariant::GUI: return std::make_unique<GUIDrawer>();
    default: return std::make_unique<ConsoleDrawer>(std::cout);
    }
}
