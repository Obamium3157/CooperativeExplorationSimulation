#ifndef COOPERATIVEEXPLORATIONSIMULATION_AGENT_H
#define COOPERATIVEEXPLORATIONSIMULATION_AGENT_H

#include <memory>

#include "../environment/Cell.h"
#include "../environment/Grid.h"


class AgentContext;
class DataBus;

class Agent
{
public:
    explicit Agent(size_t id, const Point& dimensions, const Cell& positionCell,
                   double perceptionRadius, AgentContext& context, DataBus& dataBus);
    virtual ~Agent() = default;

    size_t GetId() const noexcept;
    const Grid& GetLocalBeliefMap() const noexcept;
    const Cell& GetPosition() const noexcept;

    virtual void Act();
    void ApplyGbm();

    void Perceive();

protected:
    AgentContext& m_context;
    DataBus& m_dataBus;

private:
    size_t m_id;
    Grid m_localBeliefMap;
    Cell m_currentCell;
    double m_perceptionRadius;
};


#endif //COOPERATIVEEXPLORATIONSIMULATION_AGENT_H
