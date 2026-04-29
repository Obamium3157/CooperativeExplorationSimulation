#ifndef COOPERATIVEEXPLORATIONSIMULATION_AGENTCONTEXT_H
#define COOPERATIVEEXPLORATIONSIMULATION_AGENTCONTEXT_H

#include <map>
#include <memory>

#include "../environment/Grid.h"

class Agent;
class AgentInitializationException;

class AgentContext {
public:
    ~AgentContext();
    /**
     * @throws AgentInitializationException
     */
    explicit AgentContext(const Grid& map, const std::vector<Point>& agentPositions);
    const Agent* TryGetAgent(unsigned int id) const noexcept;

private:
    /**
     * @throws std::out_of_range if there is no such agent
     */
    const Agent* GetAgent(unsigned int id) const;

    static unsigned int s_maxId;
    std::map<unsigned int, std::unique_ptr<Agent>> m_agentById;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_AGENTCONTEXT_H
