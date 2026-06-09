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
    const std::unordered_map<size_t, const Grid*>& GetPendingLbms() const noexcept;

    void BroadcastGbm(const Grid& gbm);
    const Grid* GetGbm() const noexcept;

    void SubmitTarget(size_t agentId, const Point& target);
    std::optional<Point> ReceiveTarget(size_t agentId) const;

    void Reset();

private:
    size_t m_agentCount;
    std::unordered_map<size_t, const Grid*> m_pendingLbms;
    const Grid* m_currentGbm = nullptr;
    std::unordered_map<size_t, Point> m_pendingTargets;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_DATABUS_H
