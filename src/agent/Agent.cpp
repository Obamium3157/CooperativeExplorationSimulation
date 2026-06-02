#include "Agent.h"

#include <iostream>

Agent::Agent(const Point& dimensions, const Cell& position, AgentContext& context)
    : m_localBeliefMap(Grid(dimensions))
    , m_position(position)
    , m_context(context)
{
}

const Grid& Agent::GetLocalBeliefMap() const
{
    return m_localBeliefMap;
}

void Agent::Act() const
{
    std::cout << "Common agent things\n";
}