#ifndef COOPERATIVEEXPLORATIONSIMULATION_GRID_H
#define COOPERATIVEEXPLORATIONSIMULATION_GRID_H

#include <vector>
#include "Cell.h"
using GridMatrix = std::vector<std::vector<Cell>>;

class Grid
{
public:
    Grid(unsigned int width, unsigned int height);
    explicit Grid(GridMatrix matrix);

    const GridMatrix& GetGridMatrix() const;
    Cell GetCell(unsigned int x, unsigned int y) const;
    void SetCell(unsigned int x, unsigned int y, const Cell& cell);
    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

private:
    GridMatrix m_grid;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_GRID_H
