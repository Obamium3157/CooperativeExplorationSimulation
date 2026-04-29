#ifndef COOPERATIVEEXPLORATIONSIMULATION_CONSOLEDRAWER_H
#define COOPERATIVEEXPLORATIONSIMULATION_CONSOLEDRAWER_H

#include <iosfwd>
#include "IDrawable.h"

struct Cell;

class ConsoleDrawer : public IDrawable
{
public:
    explicit ConsoleDrawer(std::ostream& out);
    void Draw(const Grid& grid) override;

private:
    void DrawHorizontalBorder(unsigned int width) const;
    void DrawCell(const Cell& cell) const;

    std::ostream& m_out;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_CONSOLEDRAWER_H
