#ifndef COOPERATIVEEXPLORATIONSIMULATION_AGENT_H
#define COOPERATIVEEXPLORATIONSIMULATION_AGENT_H

#include <optional>
#include <vector>

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
    bool HasArrived() const noexcept;

    virtual void Act();
    void ApplyGbm();
    void ReceiveAndPlanTarget();
    void Step();

    void Perceive();

protected:
    double GetPerceptionRadius() const noexcept;

    AgentContext& m_context;
    DataBus& m_dataBus;

private:
    size_t m_id;
    Grid m_localBeliefMap;
    Cell m_currentCell;
    std::vector<Point> m_currentPath;
    double m_perceptionRadius;
    bool m_hasArrived = true;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_AGENT_H
