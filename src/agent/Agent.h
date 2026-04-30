#ifndef COOPERATIVEEXPLORATIONSIMULATION_AGENT_H
#define COOPERATIVEEXPLORATIONSIMULATION_AGENT_H

#include <memory>
#include "../environment/Cell.h"
#include "../environment/Grid.h"
#include "roles/IRole.h"

enum class RoleVariant
{
    Explorer,
    Coordinator,
};

class AgentContext;

class Agent {
public:
    explicit Agent(const Point& dimensions, const Cell& position, AgentContext& context, RoleVariant variant);

    Grid GetLocalBeliefMap() const;
    void Act() const;

    void BecomeCoordinator();
private:
    std::unique_ptr<IRole> MakeRole(RoleVariant variant);

    Grid m_localBeliefMap;
    Cell m_position;
    AgentContext& m_context;
    std::unique_ptr<IRole> m_role;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_AGENT_H
