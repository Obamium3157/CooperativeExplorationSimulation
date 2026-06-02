#ifndef COOPERATIVEEXPLORATIONSIMULATION_AGENTCONTEXT_H
#define COOPERATIVEEXPLORATIONSIMULATION_AGENTCONTEXT_H

#include <map>
#include <memory>

#include "Coordinator.h"
#include "../environment/Grid.h"

class Agent;
class AgentInitializationException;
class CoordinatorAssignationException;

class AgentContext {
public:
    /**
     * @throws AgentInitializationException
     */
    explicit AgentContext(const Grid& map, const std::vector<Point>& agentPositions, size_t coordinatorIndex);
    ~AgentContext() = default;

    const Agent* TryGetAgent(unsigned int id) const noexcept;

    const Coordinator* GetCoordinator() const noexcept;

    void IterateOverAgents();

private:
    /**
     * @throws std::out_of_range if there is no such agent
     */
    const Agent* GetAgent(unsigned int id) const;

    static unsigned int s_maxId;
    std::map<unsigned int, std::unique_ptr<Agent>> m_agentById;
    Coordinator* m_coordinator = nullptr;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_AGENTCONTEXT_H
