#include "Coordinator.h"

#include <iostream>

Coordinator::Coordinator(const Point& dimensions, const Cell& position, AgentContext& context)
    : Agent(dimensions, position, context)
    , m_globalBeliefMap(GetLocalBeliefMap())
{
}

void Coordinator::Act() const
{
    std::cout << "Some coordinator stuff\n";
}

const Grid& Coordinator::GetGlobalBeliefMap() const
{
    return m_globalBeliefMap;
}
