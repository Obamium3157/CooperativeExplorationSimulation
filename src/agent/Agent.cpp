#include "Agent.h"

Agent::Agent(const Point& dimensions, const Cell& position, AgentContext& context)
    : m_localBeliefMap(Grid(dimensions))
    , m_position(position)
    , m_context(context)
{
}
