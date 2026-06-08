#include "Coordinator.h"

#include <ranges>

#include "DataBus.h"

Coordinator::Coordinator(const size_t id, const Point& dimensions, const Cell& positionCell,
                         const double perceptionRadius, AgentContext& context, DataBus& dataBus)
    : Agent(id, dimensions, positionCell, perceptionRadius, context, dataBus)
    , m_globalBeliefMap(dimensions)
{
}

void Coordinator::SynchronizeGlobalMap()
{
    for (const auto& lbm : m_dataBus.GetPendingLbms() | std::views::values)
    {
        m_globalBeliefMap.MergeFrom(lbm);
    }
    m_frontiers = Frontier::ComputeFrontiers(m_globalBeliefMap);
    m_dataBus.BroadcastGbm(m_globalBeliefMap);
}

const Grid& Coordinator::GetGlobalBeliefMap() const noexcept
{
    return m_globalBeliefMap;
}

const std::vector<Point>& Coordinator::GetFrontiers() const noexcept
{
    return m_frontiers;
}
