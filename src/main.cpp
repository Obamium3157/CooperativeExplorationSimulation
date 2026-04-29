#include <iostream>

#include "environment/Grid.h"
#include "simulation/Simulation.h"

int main()
{
    try
    {
        const Simulation simulation("test_map1.txt", DrawableVariant::Console);
        simulation.Run();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }
}
