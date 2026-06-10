#ifndef COOPERATIVEEXPLORATIONSIMULATION_AGENTCONTEXT_H
#define COOPERATIVEEXPLORATIONSIMULATION_AGENTCONTEXT_H

#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "Coordinator.h"
#include "DataBus.h"
#include "../environment/Grid.h"

class Agent;
class AgentInitializationException;
class CoordinatorAssignationException;

class AgentContext
{
public:
    /**
     * @throws AgentInitializationException
     * @throws CoordinatorAssignationException
     */
    explicit AgentContext(const Grid& map, const std::vector<Point>& agentPositions,
                          size_t coordinatorIndex);
    ~AgentContext() = default;

    const Agent* TryGetAgent(size_t id) const noexcept;
    const Coordinator* GetCoordinator() const noexcept;
    const Grid& GetMap() const noexcept;
    size_t GetSimulationTime() const noexcept;
    std::vector<std::pair<Point, bool>> GetAgentInfos() const;

    void IterateOverAgents();

private:
    void PerceiveAll();
    void SynchronizeGlobalBeliefMap();
    void AssignTargets();
    void DistributeGlobalBeliefMap();
    void DistributeTargets();
    void MoveAllToTargets();
    bool AllAgentsArrived() const;

    /**
     * @throws std::out_of_range if there is no agent with the given id
     */
    const Agent* GetAgent(size_t id) const;

    static inline size_t s_maxId = 0;

    DataBus m_dataBus;
    std::map<size_t, std::unique_ptr<Agent>> m_agentById;
    Coordinator* m_coordinator = nullptr;
    const Grid& m_map;

    size_t m_pendingAgentCount = 0;
    size_t m_simulationTime = 0;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_AGENTCONTEXT_H
