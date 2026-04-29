#ifndef COOPERATIVEEXPLORATIONSIMULATION_GRID_H
#define COOPERATIVEEXPLORATIONSIMULATION_GRID_H

#include <vector>
#include "Cell.h"

using GridMatrix = std::vector<std::vector<Cell>>;

class Grid
{
public:
    explicit Grid(const Point& dimensions);
    explicit Grid(GridMatrix matrix);

    const GridMatrix& GetGridMatrix() const;
    Cell GetCell(const Point& point) const;
    void SetCell(const Point& point, const Cell& cell);
    Point GetDimensions() const;

private:
    GridMatrix m_grid;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_GRID_H
