#ifndef COOPERATIVEEXPLORATIONSIMULATION_SIMULATION_H
#define COOPERATIVEEXPLORATIONSIMULATION_SIMULATION_H

#include <filesystem>
#include "../agent/AgentContext.h"
#include "../environment/Grid.h"
#include "../ui/IDrawable.h"

class ReadGridException;
class AgentInitializationException;

enum class DrawableVariant
{
    Console,
    GUI,
};

class Simulation
{
public:
    explicit Simulation(
        const std::filesystem::path& filename,
        const std::vector<Point>& agentPositions,
        size_t r,
        double delta,
        DrawableVariant variant,
        unsigned int sleepTime);

    size_t Run() const;
private:
    /**
     * @throws std::runtime_error if file does not exist
     * @throws ReadGridException if file contains undefined character
     */
    static GridMatrix LoadGridFromFile(const std::filesystem::path& filename);
    static std::unique_ptr<IDrawable> MakeDrawable(DrawableVariant variant, unsigned int sleepTime);
    std::vector<std::pair<Point, char>> BuildAgentOverlay() const;

    Grid m_map;
    std::unique_ptr<AgentContext> m_context;
    std::unique_ptr<IDrawable> m_drawable;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_SIMULATION_H
