#include <format>
#include <ranges>
#include "../exceptions/AgentInitializationException.h"
#include "../exceptions/CoordinatorAssignationException.h"
#include "AgentContext.h"
#include "Agent.h"

unsigned int AgentContext::s_maxId = 0;

AgentContext::~AgentContext() = default;

AgentContext::AgentContext(const Grid& map, const std::vector<Point>& agentPositions)
{
    const auto dimensions = map.GetDimensions();

    auto createAndRegister = [&](auto agentPtr)
    {
        m_agentById.emplace(s_maxId++, std::move(agentPtr));
    };

    for (auto& agentPosition : agentPositions)
    {
        if (map.GetCell(agentPosition).state == CellState::Obstacle)
        {
            throw AgentInitializationException(
                "Tried to place agent on the position "
                + std::string(agentPosition)
                + ", which is occupied by an obstacle");
        }
        createAndRegister(std::make_unique<Agent>(dimensions, Cell{agentPosition, CellState::OccupiedByAgent}, *this,
                                                  RoleVariant::Explorer));
    }

    AssignCoordinator();
}

const Agent* AgentContext::TryGetAgent(const unsigned int id) const noexcept
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

const Agent* AgentContext::TryGetCoordinator() const noexcept
{
    try
    {
        return GetCoordinator();
    }
    catch (...)
    {
        return nullptr;
    }
}

void AgentContext::IterateOverAgents()
{
    for (const auto& agent : m_agentById | std::views::values)
    {
        agent->Act();
    }
}

const Agent* AgentContext::GetAgent(const unsigned int id) const
{
    const auto it = m_agentById.find(id);
    if (it == m_agentById.end())
    {
        throw std::out_of_range("Agent with id " + std::to_string(id) + " not found");
    }

    return it->second.get();
}

const Agent* AgentContext::GetCoordinator() const
{
    if (m_coordinatorId == std::nullopt)
    {
        throw std::out_of_range("Coordinator is not assigned");
    }

    auto it = m_agentById.find(m_coordinatorId.value());
    if (it == m_agentById.end())
    {
        throw std::out_of_range("Coordinator with id " + std::to_string(m_coordinatorId.value()) + " not found");
    }

    return it->second.get();
}

void AgentContext::AssignCoordinator()
{
    const unsigned int coordinatorId = s_maxId - 1;

    const auto it = m_agentById.find(coordinatorId);
    if (it == m_agentById.end())
    {
        throw CoordinatorAssignationException("No agent with id " + std::to_string(s_maxId));
    }

    it->second->BecomeCoordinator();
    m_coordinatorId = coordinatorId;
}
