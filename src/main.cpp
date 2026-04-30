#include <iostream>

#include "environment/Grid.h"
#include "simulation/Simulation.h"

int main()
{
    try
    {
        const Simulation simulation(
            "test_map2.txt",
            {
                Point{1, 1},
                Point{1, 1},
                Point{1, 1},
                Point{1, 1},
            },
            DrawableVariant::Console);
        simulation.Run();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }
}
