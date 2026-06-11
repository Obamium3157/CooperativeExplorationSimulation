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
        500);

    return simulation.Run();
}

size_t RunSimulation(const std::filesystem::path& filename, const std::vector<Point>& positions,
                     const size_t r, const double delta)
{
    const Simulation simulation(
        filename,
        positions,
        r,
        delta,
        DrawableVariant::GUI,
        300);

    return simulation.Run();
}

int main()
{
    try
    {
        // RunSimulation("open_field.txt", {Point{0, 0}, Point{24, 0}, Point{0, 24}, Point{24, 24}}, 5, 2 * 5);
        // RunSimulation("open_field.txt", Point{0, 0}, 6, 5, 5 * 2);
        // RunSimulation("snail.txt", Point{0, 0}, 1, 5, 5 * 2);
        RunSimulation("test_map2.txt", {Point{1, 1}, Point{15, 12}}, 5, 2 * 5);
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << "\n";
    }
}
