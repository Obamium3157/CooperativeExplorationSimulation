#ifndef COOPERATIVEEXPLORATIONSIMULATION_COORDINATOR_H
#define COOPERATIVEEXPLORATIONSIMULATION_COORDINATOR_H

#include "Agent.h"

class Coordinator : public Agent
{
public:
    explicit Coordinator(size_t id, const Point& dimensions, const Cell& positionCell,
                         double perceptionRadius, AgentContext& context, DataBus& dataBus);

    void SynchronizeGlobalMap();
    const Grid& GetGlobalBeliefMap() const noexcept;

private:
    Grid m_globalBeliefMap;
};


#endif //COOPERATIVEEXPLORATIONSIMULATION_COORDINATOR_H
