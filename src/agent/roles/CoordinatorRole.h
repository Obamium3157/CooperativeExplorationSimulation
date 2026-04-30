#ifndef COOPERATIVEEXPLORATIONSIMULATION_COORDINATORROLE_H
#define COOPERATIVEEXPLORATIONSIMULATION_COORDINATORROLE_H

#include "IRole.h"
#include "../../environment/Grid.h"

class Agent;

class CoordinatorRole : public IRole
{
public:
    explicit CoordinatorRole(Agent& agent);

    void Act() override;
    const Grid& GetGlobalBeliefMap();

private:
    Agent& m_agent;
    Grid m_globalBeliefMap;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_COORDINATORROLE_H
