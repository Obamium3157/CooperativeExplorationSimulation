#ifndef COOPERATIVEEXPLORATIONSIMULATION_GRID_H
#define COOPERATIVEEXPLORATIONSIMULATION_GRID_H

#include <vector>
#include "Cell.h"

using GridMatrix = std::vector<std::vector<Cell>>;

class Grid
{
public:
    explicit Grid(const Point& dimensions);

    explicit Grid(const GridMatrix& matrix);
    explicit Grid(GridMatrix&& matrix);

    Grid(const Grid& other) = default;
    Grid(Grid&& other) = default;

    Grid& operator=(const Grid& other) = default;
    Grid& operator=(Grid&& other) = default;

    const GridMatrix& GetGridMatrix() const;
    Cell GetCell(const Point& point) const;
    void SetCell(const Point& point, const Cell& cell);
    Point GetDimensions() const;

    void MergeFrom(const Grid& other);

private:
    GridMatrix m_grid;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_GRID_H
