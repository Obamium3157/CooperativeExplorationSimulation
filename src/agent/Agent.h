#ifndef COOPERATIVEEXPLORATIONSIMULATION_AGENT_H
#define COOPERATIVEEXPLORATIONSIMULATION_AGENT_H

#include "../environment/Cell.h"
#include "../environment/Grid.h"

class Agent {
public:
    explicit Agent(unsigned int width, unsigned int height, Cell position);

private:
    Grid m_localBeliefMap;
    Cell m_position;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_AGENT_H
