#include <iostream>

#include "simulation/Simulation.h"

int main()
{
    try
    {
        const Simulation simulation(
            "test_map2.txt",
            {
                Point{8, 10},
                Point{8, 10},
                Point{8, 10},
                Point{8, 10},
            },
            DrawableVariant::Console);
        simulation.Run();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }
}
