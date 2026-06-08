#ifndef COOPERATIVEEXPLORATIONSIMULATION_DATABUS_H
#define COOPERATIVEEXPLORATIONSIMULATION_DATABUS_H

#include <optional>
#include <unordered_map>

#include "../environment/Grid.h"

class DataBus
{
public:
    explicit DataBus(size_t agentCount);

    void SubmitLbm(size_t agentId, const Grid& lbm);
    bool AreAllLbmsReady() const noexcept;
    const std::unordered_map<size_t, Grid>& GetPendingLbms() const noexcept;

    void BroadcastGbm(const Grid& gbm);
    const std::optional<Grid>& GetGbm() const noexcept;

    void Reset();

private:
    size_t m_agentCount;
    std::unordered_map<size_t, Grid> m_pendingLbms;
    std::optional<Grid> m_currentGbm;
};


#endif //COOPERATIVEEXPLORATIONSIMULATION_DATABUS_H
