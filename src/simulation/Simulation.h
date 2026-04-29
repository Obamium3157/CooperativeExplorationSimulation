#ifndef COOPERATIVEEXPLORATIONSIMULATION_SIMULATION_H
#define COOPERATIVEEXPLORATIONSIMULATION_SIMULATION_H

#include <filesystem>
#include "../environment/Grid.h"
#include "../ui/IDrawable.h"

class ReadGridException;

enum class DrawableVariant
{
    Console,
    GUI,
};

class Simulation
{
public:
    explicit Simulation(const std::filesystem::path& filename, DrawableVariant variant);

    void Run() const;
private:
    /**
     * @throws std::runtime_error if file does not exist
     * @throws ReadGridException if file contains undefined character
     */
    static GridMatrix LoadGridFromFile(const std::filesystem::path& filename);
    static std::unique_ptr<IDrawable> MakeDrawable(DrawableVariant variant);

    std::unique_ptr<IDrawable> m_drawable;
    Grid m_map;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_SIMULATION_H
