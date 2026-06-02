#include "Agent.h"

#include <iostream>

Agent::Agent(const size_t id, const Point& dimensions, const Cell& position, AgentContext& context)
    : m_id(id)
    , m_localBeliefMap(Grid(dimensions))
    , m_position(position)
    , m_context(context)
{
}

size_t Agent::GetId() const noexcept
{
    return m_id;
}

const Grid& Agent::GetLocalBeliefMap() const noexcept
{
    return m_localBeliefMap;
}

const Cell& Agent::GetPosition() const noexcept
{
    return m_position;
}

void Agent::Act() const
{
    std::cout << "Common agent things (" << m_id << ")" << "\n";
}