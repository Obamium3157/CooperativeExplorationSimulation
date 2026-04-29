#include "environment/Grid.h"

int main()
{
    const Grid grid(10, 10, DrawableVariant::Console);
    grid.Render();
}