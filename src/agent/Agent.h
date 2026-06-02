#ifndef COOPERATIVEEXPLORATIONSIMULATION_AGENT_H
#define COOPERATIVEEXPLORATIONSIMULATION_AGENT_H

#include <memory>

#include "../environment/Cell.h"
#include "../environment/Grid.h"


using AgentId = size_t;

class AgentContext;

class Agent {
public:
    explicit Agent(const Point& dimensions, const Cell& position, AgentContext& context);
    virtual ~Agent() = default;

    const Grid& GetLocalBeliefMap() const;

    virtual void Act() const;

private:
    Grid m_localBeliefMap;

protected:
    Cell m_position;
    AgentContext& m_context;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_AGENT_H
