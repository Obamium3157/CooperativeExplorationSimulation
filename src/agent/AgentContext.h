#ifndef COOPERATIVEEXPLORATIONSIMULATION_AGENTCONTEXT_H
#define COOPERATIVEEXPLORATIONSIMULATION_AGENTCONTEXT_H

#include <map>
#include <memory>

#include "../environment/Grid.h"

class Agent;
class AgentInitializationException;
class CoordinatorAssignationException;

class AgentContext {
public:
    ~AgentContext();
    /**
     * @throws AgentInitializationException
     */
    explicit AgentContext(const Grid& map, const std::vector<Point>& agentPositions);
    const Agent* TryGetAgent(unsigned int id) const noexcept;
    const Agent* TryGetCoordinator() const noexcept;

    void IterateOverAgents();

private:
    /**
     * @throws std::out_of_range if there is no such agent
     */
    const Agent* GetAgent(unsigned int id) const;

    /**
     * @throws std::out_of_range if the coordinator is not assigned
     */
    const Agent* GetCoordinator() const;

    /**
     * @throws CoordinatorAssignationException if failed trying to assign a coordinator
     */
    void AssignCoordinator();

    static unsigned int s_maxId;
    std::map<unsigned int, std::unique_ptr<Agent>> m_agentById;
    std::optional<unsigned int> m_coordinatorId = std::nullopt;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_AGENTCONTEXT_H
