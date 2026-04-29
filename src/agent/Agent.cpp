#include "Agent.h"

Agent::Agent(const unsigned int width, const unsigned int height, const Cell position)
    : m_localBeliefMap(Grid(width, height))
    , m_position(position)
{
}
