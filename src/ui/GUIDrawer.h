#ifndef COOPERATIVEEXPLORATIONSIMULATION_GUIDRAWER_H
#define COOPERATIVEEXPLORATIONSIMULATION_GUIDRAWER_H

#include "IDrawable.h"

class GUIDrawer : public IDrawable
{
public:
    void Draw(const Grid& grid) override;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_GUIDRAWER_H
