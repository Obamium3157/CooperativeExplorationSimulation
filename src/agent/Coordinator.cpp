#include "Coordinator.h"

#include <iostream>

Coordinator::Coordinator(const size_t id, const Point& dimensions, const Cell& position, AgentContext& context)
    : Agent(id, dimensions, position, context)
    , m_globalBeliefMap(GetLocalBeliefMap())
{
}

void Coordinator::Act() const
{
    std::cout << "Some coordinator stuff (" << GetId() << ")" << "\n";
}

const Grid& Coordinator::GetGlobalBeliefMap() const
{
    return m_globalBeliefMap;
}
