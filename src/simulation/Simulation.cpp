#include "Simulation.h"

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <ranges>

#include "../agent/Agent.h"
#include "../exceptions/AgentInitializationException.h"
#include "../exceptions/ReadGridException.h"
#include "../ui/DrawableCharacters.h"
#include "../ui/ConsoleDrawer.h"
#include "../ui/GUIDrawer.h"

namespace
{
    std::unordered_map<char, CellState> charToCellStateMap = {
        {DrawableCharacter::Unknown, CellState::Unknown},
        {DrawableCharacter::Free, CellState::Free},
        {DrawableCharacter::Obstacle, CellState::Obstacle},
        {DrawableCharacter::Agent, CellState::OccupiedByAgent},
    };

    CellState GetSellStateByChar(const char ch)
    {
        const auto it = charToCellStateMap.find(ch);
        if (it == charToCellStateMap.end())
        {
            throw ReadGridException("No such character: " + std::string(1, ch));
        }

        return it->second;
    }
}

Simulation::Simulation(const std::filesystem::path& filename,
        const std::vector<Point>& agentPositions,
        const size_t r,
        const double delta,
        const DrawableVariant variant,
        const unsigned int sleepTime)
    : m_map(LoadGridFromFile(filename))
    , m_context(std::make_unique<AgentContext>(m_map, agentPositions, agentPositions.size() - 1, r, delta))
    , m_drawable(MakeDrawable(variant, sleepTime))
{
}

size_t Simulation::Run() const
{
    const auto onStep = [this]()
    {
        if (const auto coordinator = m_context->GetCoordinator(); coordinator)
        {
            m_drawable->Draw(coordinator->GetGlobalBeliefMap(), BuildAgentOverlay());
        }
    };

    m_context->IterateOverAgents(onStep);
    while (!m_context->GetCoordinator()->GetFrontiers().empty())
    {
        m_context->IterateOverAgents(onStep);
    }
    onStep();

    m_drawable->OnSimulationFinished();

    return m_context->GetSimulationTime();
}

GridMatrix Simulation::LoadGridFromFile(const std::filesystem::path& filename)
{
    GridMatrix matrix;

    std::ifstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + filename.string());
    }

    std::string line;
    size_t y = 0;
    size_t expectedWidth = 0;

    while (std::getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }
        if (line.empty())
        {
            continue;
        }

        matrix.emplace_back();
        size_t col = 0;

        for (const char ch : line)
        {
            if (std::isspace(ch))
            {
                continue;
            }

            const auto state = GetSellStateByChar(ch);
            matrix[y].emplace_back(Cell{Point{col, y}, state});
            col++;
        }

        if (expectedWidth == 0)
        {
            expectedWidth = col;
        }
        else if (col != expectedWidth)
        {
            throw ReadGridException("Inconsistent row lengths at line " + std::to_string(y) +
                "Expected: " + std::to_string(expectedWidth) +
                ", Got: " + std::to_string(col));
        }
        y++;
    }

    if (matrix.empty())
    {
        throw ReadGridException("Grid file is empty or contains no valid cells.");
    }

    return matrix;
}

std::unique_ptr<IDrawable> Simulation::MakeDrawable(const DrawableVariant variant, const unsigned int sleepTime)
{
    switch (variant)
    {
    case DrawableVariant::Console: return std::make_unique<ConsoleDrawer>(std::cout, sleepTime);
    case DrawableVariant::GUI: return std::make_unique<GUIDrawer>(sleepTime);
    default: return std::make_unique<ConsoleDrawer>(std::cout, sleepTime);
    }
}

std::vector<AgentOverlayEntry> Simulation::BuildAgentOverlay() const
{
    const auto agentInfos = m_context->GetAgentInfos();

    std::vector<AgentOverlayEntry> overlay;
    overlay.reserve(agentInfos.size());

    for (const auto& [position, isCoordinator, target] : agentInfos)
    {
        if (isCoordinator)
        {
            overlay.emplace_back(position, DrawableCharacter::Coordinator, target);
        }
        else
        {
            overlay.emplace_back(position, DrawableCharacter::Agent, target);
        }
    }

    return overlay;
}
