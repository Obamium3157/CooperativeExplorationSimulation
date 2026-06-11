#include <iostream>

#include "simulation/Simulation.h"

size_t RunSimulation(const std::filesystem::path& filename, const Point initialPos,
                     const size_t amount, const size_t r, const double delta)
{
    const Simulation simulation(
        filename,
        std::vector<Point>(amount, initialPos),
        r,
        delta,
        DrawableVariant::GUI,
        300);

    return simulation.Run();
}

int main()
{
    const std::vector<size_t> radiusRange{1, 2, 5, 8, 10};
    try
    {
        // for (const size_t radius : radiusRange)
        // {
        //     const std::vector<double> deltaRange{0.0, 1.0, static_cast<double>(radius), 2.0 * static_cast<double>(radius), 3.0 * static_cast<double>(radius)};
        //
        //     for (const double delta : deltaRange)
        //     {
        //         std::cout << "delta = " << delta << "\n";
        //         for (size_t i = 1; i <= 10; ++i)
        //         {
        //             std::cout << "Radius: " << radius
        //                 << "\tAmount: " << i
        //                 << "\tResult: " << RunSimulation("brush.txt", Point{0, 0}, i, radius, 3.0 * radius) << "\n";
        //         }
        //         std::cout << "\n";
        //     }
        // }
        RunSimulation("open_field.txt", Point{1, 1}, 5, 5, 2 * 5);
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }
}
