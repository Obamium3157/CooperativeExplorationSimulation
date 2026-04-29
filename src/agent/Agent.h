#ifndef COOPERATIVEEXPLORATIONSIMULATION_AGENT_H
#define COOPERATIVEEXPLORATIONSIMULATION_AGENT_H

#include "../environment/Cell.h"
#include "../environment/Grid.h"

class AgentContext;

class Agent {
public:
    explicit Agent(const Point& dimensions, const Cell& position, AgentContext& context);

private:
    Grid m_localBeliefMap;
    Cell m_position;
    AgentContext& m_context;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_AGENT_H
