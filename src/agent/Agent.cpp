#include "Agent.h"

#include "AgentContext.h"
#include "DataBus.h"
#include "../environment/Pathfinding.h"
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

bool Agent::HasArrived() const noexcept
{
    return m_hasArrived;
}

std::vector<Point> Agent::GetPath() const noexcept
{
    return m_currentPath;
}

void Agent::Act()
{
    Perceive();
    m_dataBus.SubmitLbm(m_id, m_localBeliefMap);
}

void Agent::ApplyGbm()
{
    const Grid* gbm = m_dataBus.GetGbm();
    if (!gbm)
    {
        return;
    }
    m_localBeliefMap.MergeFrom(*gbm);
}

void Agent::ReceiveAndPlanTarget()
{
    const auto target = m_dataBus.ReceiveTarget(m_id);
    if (!target.has_value())
    {
        m_hasArrived = true;
        m_currentPath.clear();
        return;
    }

    auto path = Pathfinding::FindPath(m_currentCell.position, *target, m_localBeliefMap);
    if (!path.has_value() || path->size() <= 1)
    {
        m_hasArrived = true;
        m_currentPath.clear();
        return;
    }

    m_currentPath.assign(path->begin() + 1, path->end());
    std::reverse(m_currentPath.begin(), m_currentPath.end());
    m_hasArrived = false;
}

void Agent::Step()
{
    if (m_hasArrived)
    {
        return;
    }

    m_currentCell.position = m_currentPath.back();
    m_currentPath.pop_back();

    Perceive();

    if (m_currentPath.empty())
    {
        m_hasArrived = true;
    }
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
