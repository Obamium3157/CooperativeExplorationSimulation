#include "Agent.h"

#include <iostream>

#include "AgentContext.h"
#include "../environment/Perception.h"

Agent::Agent(const size_t id, const Point& dimensions, const Cell& positionCell, const double perceptionRadius, AgentContext& context)
    : m_id(id)
    , m_localBeliefMap(Grid(dimensions))
    , m_currentCell(positionCell)
    , m_perceptionRadius(perceptionRadius)
    , m_context(context)
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
    std::cout << "Common agent things (" << m_id << ")" << "\n";
    Perceive();
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
