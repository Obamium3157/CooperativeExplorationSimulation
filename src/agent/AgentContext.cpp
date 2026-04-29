#include "AgentContext.h"
#include "Agent.h"
#include <format>
#include "../exceptions/AgentInitializationException.h"

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
        createAndRegister(std::make_unique<Agent>(dimensions, Cell{agentPosition, CellState::OccupiedByAgent}, *this));
    }
}

const Agent* AgentContext::TryGetAgent(const unsigned int id) const noexcept
{
    try
    {
        return GetAgent(id);
    }
    catch(...)
    {
        return nullptr;
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
