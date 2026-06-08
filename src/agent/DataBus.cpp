#include "DataBus.h"

DataBus::DataBus(const size_t agentCount)
    : m_agentCount(agentCount)
{
}

void DataBus::SubmitLbm(const size_t agentId, const Grid& lbm)
{
    m_pendingLbms.insert_or_assign(agentId, lbm);
}

bool DataBus::AreAllLbmsReady() const noexcept
{
    return m_pendingLbms.size() == m_agentCount;
}

const std::unordered_map<size_t, Grid>& DataBus::GetPendingLbms() const noexcept
{
    return m_pendingLbms;
}

void DataBus::BroadcastGbm(const Grid& gbm)
{
    m_currentGbm = gbm;
}

const std::optional<Grid>& DataBus::GetGbm() const noexcept
{
    return m_currentGbm;
}

void DataBus::Reset()
{
    m_pendingLbms.clear();
    m_currentGbm.reset();
}
