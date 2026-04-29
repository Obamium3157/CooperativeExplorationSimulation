#ifndef COOPERATIVEEXPLORATIONSIMULATION_SIMULATION_H
#define COOPERATIVEEXPLORATIONSIMULATION_SIMULATION_H

#include <filesystem>
#include "../environment/Grid.h"

class ReadGridException;

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
    Grid m_map;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_SIMULATION_H
