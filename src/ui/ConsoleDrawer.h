#ifndef COOPERATIVEEXPLORATIONSIMULATION_CONSOLEDRAWER_H
#define COOPERATIVEEXPLORATIONSIMULATION_CONSOLEDRAWER_H

#include <iosfwd>
#include <utility>
#include <vector>

#include "../environment/CellState.h"
#include "../environment/Point.h"
#include "IDrawable.h"

struct Cell;

class ConsoleDrawer : public IDrawable
{
public:
    explicit ConsoleDrawer(std::ostream& out);
    void Draw(const Grid& grid,
              const std::vector<std::pair<Point, char>>& agentOverlay) override;

private:
    void DrawHorizontalBorder(unsigned int width) const;
    void DrawCell(const Cell& cell) const;

    std::ostream& m_out;
};

std::ostream& operator<<(std::ostream& os, const CellState& cellState);

#endif //COOPERATIVEEXPLORATIONSIMULATION_CONSOLEDRAWER_H
