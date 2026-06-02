#ifndef COOPERATIVEEXPLORATIONSIMULATION_COORDINATOR_H
#define COOPERATIVEEXPLORATIONSIMULATION_COORDINATOR_H

#include "Agent.h"

class Coordinator : public Agent
{
public:
    explicit Coordinator(size_t id, const Point& dimensions, const Cell& positionCell, double perceptionRadius, AgentContext& context);

    void Act() override;
    const Grid& GetGlobalBeliefMap() const;

private:
    Grid m_globalBeliefMap;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_COORDINATOR_H
