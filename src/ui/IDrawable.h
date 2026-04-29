#ifndef COOPERATIVEEXPLORATIONSIMULATION_IDRAWABLE_H
#define COOPERATIVEEXPLORATIONSIMULATION_IDRAWABLE_H

class Grid;

class IDrawable
{
public:
    virtual ~IDrawable() = default;

    virtual void Draw(const Grid& grid) = 0;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_IDRAWABLE_H
