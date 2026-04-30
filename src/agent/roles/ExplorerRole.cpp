#include "ExplorerRole.h"

#include <iostream>

ExplorerRole::ExplorerRole(Agent& agent)
    : m_agent(agent)
{
}

void ExplorerRole::Act()
{
    std::cout << "Exploring...\n";
}
