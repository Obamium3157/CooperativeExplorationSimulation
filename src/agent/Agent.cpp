#include "Agent.h"

#include <iostream>

#include "AgentContext.h"
#include "DataBus.h"
#include "../environment/Perception.h"

Agent::Agent(const size_t id, const Point& dimensions, const Cell& positionCell,
             const double perceptionRadius, AgentContext& context, DataBus& dataBus)
    : m_context(context)
    , m_dataBus(dataBus)
    , m_id(id)
    , m_localBeliefMap(Grid(dimensions))
    , m_currentCell(positionCell)
    , m_perceptionRadius(perceptionRadius)
{
}

size_t Agent::GetId() const noexcept
{
    return m_id;
}

const Grid& Agent::GetLocalBeliefMap() const noexcept
{
    return m_localBeliefMap;
}

const Cell& Agent::GetPosition() const noexcept
{
    return m_currentCell;
}

void Agent::Act()
{
    Perceive();
    m_dataBus.SubmitLbm(m_id, m_localBeliefMap);
}

void Agent::ApplyGbm()
{
    const auto& gbm = m_dataBus.GetGbm();
    if (!gbm.has_value())
    {
        return;
    }
    m_localBeliefMap.MergeFrom(*gbm);
}

void Agent::Perceive()
{
    const auto perceivedCells = Perception::GetPerceivedCells(
        m_currentCell.position, m_perceptionRadius, m_context.GetMap());

    for (const auto& cell : perceivedCells)
    {
        m_localBeliefMap.SetCell(cell.position, cell);
    }
}

double Agent::GetPerceptionRadius() const noexcept
{
    return m_perceptionRadius;
}
