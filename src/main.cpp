#include <iostream>

#include "environment/Grid.h"
#include "simulation/Simulation.h"

int main()
{
    try
    {
        const Simulation simulation("test_map1.txt", {Point{1, 1}, Point{2, 1}, Point{3, 1}, Point{4, 1}}, DrawableVariant::Console);
        simulation.Run();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }
}
