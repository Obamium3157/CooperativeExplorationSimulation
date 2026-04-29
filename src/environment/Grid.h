#ifndef COOPERATIVEEXPLORATIONSIMULATION_GRID_H
#define COOPERATIVEEXPLORATIONSIMULATION_GRID_H

#include <memory>
#include <vector>
#include "Cell.h"

#include "../ui/IDrawable.h"

enum class DrawableVariant
{
    Console,
    GUI,
};

using GridMatrix = std::vector<std::vector<Cell>>;

class Grid
{
public:
    Grid(unsigned int width, unsigned int height, DrawableVariant variant);

    const GridMatrix& GetGridMatrix() const;
    Cell GetCell(unsigned int x, unsigned int y) const;
    void SetCell(unsigned int x, unsigned int y, const Cell& cell);
    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

    void Render() const;

private:
    static std::unique_ptr<IDrawable> MakeDrawable(DrawableVariant variant);

    GridMatrix m_grid;
    std::unique_ptr<IDrawable> m_drawable;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_GRID_H
