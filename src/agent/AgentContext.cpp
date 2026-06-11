#include <format>
#include <ranges>

#include "AgentContext.h"
#include "Agent.h"
#include "../exceptions/AgentInitializationException.h"
#include "../exceptions/CoordinatorAssignationException.h"

AgentContext::AgentContext(const Grid& map, const std::vector<Point>& agentPositions,
                           const size_t coordinatorIndex,
                           const size_t agentPerceptionRadius,
                           const double delta)
    : m_dataBus(agentPositions.size())
    , m_map(map)
{
    if (coordinatorIndex >= agentPositions.size())
    {
        throw CoordinatorAssignationException(
            "Coordinator index " + std::to_string(coordinatorIndex)
            + " is out of range");
    }

    const auto dimensions = map.GetDimensions();

    for (size_t i = 0; i < agentPositions.size(); ++i)
    {
        const auto& position = agentPositions[i];

        if (map.GetCell(position).state == CellState::Obstacle)
        {
            throw AgentInitializationException(
                "Tried to place agent on the position "
                + std::string(position)
                + ", which is occupied by an obstacle");
        }

        const auto agentId = i;

        if (i == coordinatorIndex)
        {
            auto coordinator = std::make_unique<Coordinator>(
                agentId, dimensions, Cell{position, CellState::OccupiedByAgent},
                agentPerceptionRadius, *this, m_dataBus, delta);
            m_coordinator = coordinator.get();
            m_agentById.emplace(agentId, std::move(coordinator));
        }
        else
        {
            m_agentById.emplace(agentId,
                std::make_unique<Agent>(agentId, dimensions, Cell{position, CellState::OccupiedByAgent},
                agentPerceptionRadius, *this, m_dataBus));
        }
    }
}

const Agent* AgentContext::TryGetAgent(const size_t id) const noexcept
{
    try
    {
        return GetAgent(id);
    }
    catch (...)
    {
        return nullptr;
    }
}

const Coordinator* AgentContext::GetCoordinator() const noexcept
{
    return m_coordinator;
}

const Grid& AgentContext::GetMap() const noexcept
{
    return m_map;
}

size_t AgentContext::GetSimulationTime() const noexcept
{
    return m_simulationTime;
}

std::vector<AgentInfo> AgentContext::GetAgentInfos() const
{
    std::vector<AgentInfo> infos;
    infos.reserve(m_agentById.size());
    for (const auto& [id, agent] : m_agentById)
    {
        const bool isCoordinator = (agent.get() == m_coordinator);
        infos.emplace_back(agent->GetPosition().position, isCoordinator, agent->GetPath());
    }
    return infos;
}

void AgentContext::IterateOverAgents(const std::function<void()>& onStep)
{
    PerceiveAll();
    SynchronizeGlobalBeliefMap();
    AssignTargets();
    DistributeGlobalBeliefMap();
    DistributeTargets();
    MoveAllToTargets(onStep);
    m_dataBus.Reset();
}

void AgentContext::PerceiveAll()
{
    for (const auto& agent : m_agentById | std::views::values)
    {
        agent->Act();
    }
}

void AgentContext::SynchronizeGlobalBeliefMap()
{
    m_coordinator->SynchronizeGlobalMap();
}

void AgentContext::AssignTargets()
{
    m_coordinator->AssignTargets();
}

void AgentContext::DistributeGlobalBeliefMap()
{
    for (const auto& agent : m_agentById | std::views::values)
    {
        agent->ApplyGbm();
    }
}

void AgentContext::DistributeTargets()
{
    m_pendingAgentCount = 0;
    for (const auto& agent : m_agentById | std::views::values)
    {
        agent->ReceiveAndPlanTarget();
        if (!agent->HasArrived())
        {
            ++m_pendingAgentCount;
        }
    }
}

void AgentContext::MoveAllToTargets(const std::function<void()>& onStep)
{
    while (m_pendingAgentCount > 0)
    {
        for (const auto& agent : m_agentById | std::views::values)
        {
            if (agent->HasArrived())
            {
                continue;
            }
            agent->Step();
            if (agent->HasArrived())
            {
                --m_pendingAgentCount;
            }
        }
        ++m_simulationTime;
        if (onStep)
        {
            onStep();
        }
    }
}

bool AgentContext::AllAgentsArrived() const
{
    return m_pendingAgentCount == 0;
}

const Agent* AgentContext::GetAgent(const size_t id) const
{
    const auto it = m_agentById.find(id);
    if (it == m_agentById.end())
    {
        throw std::out_of_range("Agent with id " + std::to_string(id) + " not found");
    }

    return it->second.get();
}
