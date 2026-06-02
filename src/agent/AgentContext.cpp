#include <format>
#include <ranges>
#include "../exceptions/AgentInitializationException.h"
#include "../exceptions/CoordinatorAssignationException.h"
#include "AgentContext.h"
#include "Agent.h"

AgentContext::AgentContext(const Grid& map, const std::vector<Point>& agentPositions, const size_t coordinatorIndex)
{
    if (coordinatorIndex >= agentPositions.size())
    {
        throw CoordinatorAssignationException(
            "Coordinator index " + std::to_string(coordinatorIndex)
            + " is out of range");
    }

    const auto dimensions = map.GetDimensions();

    for (std::size_t i = 0; i < agentPositions.size(); ++i)
    {
        const auto& position = agentPositions[i];

        if (map.GetCell(position).state == CellState::Obstacle)
        {
            throw AgentInitializationException(
                "Tried to place agent on the position "
                + std::string(position)
                + ", which is occupied by an obstacle");
        }

        const auto agentId = s_maxId++;

        if (i == coordinatorIndex)
        {
            auto coordinator = std::make_unique<Coordinator>(
                agentId, dimensions, Cell{position, CellState::OccupiedByAgent}, *this);
            m_coordinator = coordinator.get();
            m_agentById.emplace(agentId, std::move(coordinator));
        }
        else
        {
            m_agentById.emplace(agentId,
                std::make_unique<Agent>(agentId, dimensions, Cell{position, CellState::OccupiedByAgent}, *this));
        }
    }
}

const Agent* AgentContext::TryGetAgent(const size_t id) const noexcept
{
    try
    {
        return GetAgent(id);
    }
    catch (...)
    {
        return nullptr;
    }
}

const Coordinator* AgentContext::GetCoordinator() const noexcept
{
    return m_coordinator;
}

void AgentContext::IterateOverAgents()
{
    for (const auto& agent : m_agentById | std::views::values)
    {
        agent->Act();
    }
}

const Agent* AgentContext::GetAgent(const size_t id) const
{
    const auto it = m_agentById.find(id);
    if (it == m_agentById.end())
    {
        throw std::out_of_range("Agent with id " + std::to_string(id) + " not found");
    }

    return it->second.get();
}
