#include "Coordinator.h"

#include <iostream>

Coordinator::Coordinator(const size_t id, const Point& dimensions, const Cell& positionCell, const double perceptionRadius, AgentContext& context)
    : Agent(id, dimensions, positionCell, perceptionRadius, context)
    , m_globalBeliefMap(GetLocalBeliefMap())
{
}

void Coordinator::Act()
{
    std::cout << "Some coordinator stuff (" << GetId() << ")" << "\n";
    Perceive();
}

const Grid& Coordinator::GetGlobalBeliefMap() const
{
    return m_globalBeliefMap;
}
