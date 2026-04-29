#include <iostream>

#include "environment/Grid.h"
#include "simulation/Simulation.h"

int main()
{
    try
    {
        const Simulation simulation("test_map1.txt", {Point{0, 1}}, DrawableVariant::Console);
        simulation.Run();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }
}
