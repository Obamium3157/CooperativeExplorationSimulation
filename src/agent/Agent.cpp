#include "Agent.h"

#include "roles/CoordinatorRole.h"
#include "roles/ExplorerRole.h"

Agent::Agent(const Point& dimensions, const Cell& position, AgentContext& context, const RoleVariant variant)
    : m_localBeliefMap(Grid(dimensions))
    , m_position(position)
    , m_context(context)
    , m_role(MakeRole(variant))
{
}

Grid Agent::GetLocalBeliefMap() const
{
    return m_localBeliefMap;
}

void Agent::Act() const
{
    m_role->Act();
}

void Agent::BecomeCoordinator()
{
    m_role = MakeRole(RoleVariant::Coordinator);
}

std::unique_ptr<IRole> Agent::MakeRole(const RoleVariant variant)
{
    switch (variant)
    {
    case RoleVariant::Explorer: return std::make_unique<ExplorerRole>(*this);
    case RoleVariant::Coordinator: return std::make_unique<CoordinatorRole>(*this);
    default: return std::make_unique<ExplorerRole>(*this);
    }
}