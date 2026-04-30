#include "CoordinatorRole.h"

#include <iostream>

#include "../Agent.h"

CoordinatorRole::CoordinatorRole(Agent& agent)
    : m_agent(agent)
    , m_globalBeliefMap(agent.GetLocalBeliefMap())
{
}

void CoordinatorRole::Act()
{
    std::cout << "Hello I'm coordinator.\n";
}
