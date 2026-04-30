#ifndef COOPERATIVEEXPLORATIONSIMULATION_EXPLORERROLE_H
#define COOPERATIVEEXPLORATIONSIMULATION_EXPLORERROLE_H

#include "IRole.h"

class Agent;

class ExplorerRole : public IRole
{
public:
    explicit ExplorerRole(Agent& agent);

    void Act() override;

private:
    Agent& m_agent;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_EXPLORERROLE_H
