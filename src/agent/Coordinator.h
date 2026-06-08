#ifndef COOPERATIVEEXPLORATIONSIMULATION_COORDINATOR_H
#define COOPERATIVEEXPLORATIONSIMULATION_COORDINATOR_H

#include <vector>

#include "Agent.h"
#include "../environment/Frontier.h"

class Coordinator : public Agent
{
public:
    explicit Coordinator(size_t id, const Point& dimensions, const Cell& positionCell,
                         double perceptionRadius, AgentContext& context, DataBus& dataBus);

    void SynchronizeGlobalMap();

    const Grid& GetGlobalBeliefMap() const noexcept;
    const std::vector<Point>& GetFrontiers() const noexcept;

private:
    Grid m_globalBeliefMap;
    std::vector<Point> m_frontiers;
};


#endif //COOPERATIVEEXPLORATIONSIMULATION_COORDINATOR_H
