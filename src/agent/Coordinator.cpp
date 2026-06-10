#include "Coordinator.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <queue>
#include <unordered_set>

#include "AgentContext.h"
#include "DataBus.h"
#include "../environment/Frontier.h"
#include "../environment/Pathfinding.h"

namespace
{
    using CandidateMap = std::unordered_map<size_t, std::vector<Point>>;
    using PositionMap = std::unordered_map<size_t, Point>;
    using PathLengthMap = std::unordered_map<size_t, size_t>;
    using Assignment = std::unordered_map<size_t, Point>;

    double PolygonArea(std::vector<Point> points)
    {
        const size_t n = points.size();
        if (n < 3)
        {
            return 0.0;
        }

        double cx = 0.0;
        double cy = 0.0;
        for (const Point& p : points)
        {
            cx += static_cast<double>(p.x);
            cy += static_cast<double>(p.y);
        }
        cx /= static_cast<double>(n);
        cy /= static_cast<double>(n);

        std::sort(points.begin(), points.end(), [cx, cy](const Point& a, const Point& b)
        {
            return std::atan2(static_cast<double>(a.y) - cy, static_cast<double>(a.x) - cx)
                < std::atan2(static_cast<double>(b.y) - cy, static_cast<double>(b.x) - cx);
        });

        double area = 0.0;
        for (size_t k = 0; k < n; ++k)
        {
            const size_t next = (k + 1) % n;
            area += static_cast<double>(points[k].x) * static_cast<double>(points[next].y)
                - static_cast<double>(points[next].x) * static_cast<double>(points[k].y);
        }
        return std::abs(area) / 2.0;
    }

    CandidateMap FilterByManhattanDistance(const std::vector<Point>& frontiers,
                                           const PositionMap& positions)
    {
        CandidateMap candidates;

        for (const auto& [agentId, pos] : positions)
        {
            size_t minDist = SIZE_MAX;
            for (const Point& f : frontiers)
            {
                const size_t dist = ManhattanDistance(pos, f);
                if (dist < minDist)
                {
                    minDist = dist;
                    candidates[agentId].clear();
                    candidates[agentId].push_back(f);
                }
                else if (dist == minDist)
                {
                    candidates[agentId].push_back(f);
                }
            }
        }

        for (const Point& f : frontiers)
        {
            std::vector<size_t> claimants;
            for (const auto& [agentId, cands] : candidates)
            {
                if (std::any_of(cands.begin(), cands.end(),
                                [&f](const Point& p) { return f == p; }))
                {
                    claimants.push_back(agentId);
                }
            }

            if (claimants.size() <= 1)
            {
                continue;
            }

            const size_t winner = *std::min_element(claimants.begin(), claimants.end(),
                [&](const size_t a, const size_t b)
                {
                    const size_t da = ManhattanDistance(positions.at(a), f);
                    const size_t db = ManhattanDistance(positions.at(b), f);
                    return da != db ? da < db : a < b;
                });

            for (const size_t loser : claimants)
            {
                if (loser == winner)
                {
                    continue;
                }
                if (positions.at(loser) == positions.at(winner))
                {
                    continue;
                }
                auto& cands = candidates[loser];
                cands.erase(
                    std::remove_if(cands.begin(), cands.end(),
                                   [&f](const Point& p) { return f == p; }),
                    cands.end());
            }
        }

        std::erase_if(candidates, [](const auto& kv) { return kv.second.empty(); });
        return candidates;
    }

    CandidateMap FilterByPathLength(const CandidateMap& step1,
                                    const PositionMap& positions,
                                    const Grid& globalMap,
                                    PathLengthMap& outLengths)
    {
        CandidateMap step2;

        for (const auto& [agentId, cands] : step1)
        {
            const Point& agentPos = positions.at(agentId);

            std::vector<std::pair<Point, size_t>> reachable;
            for (const Point& f : cands)
            {
                const auto len = Pathfinding::FindPathLength(agentPos, f, globalMap);
                if (len.has_value())
                {
                    reachable.emplace_back(f, *len);
                }
            }

            if (reachable.empty())
            {
                continue;
            }

            const size_t minLen = std::min_element(reachable.begin(), reachable.end(),
                [](const auto& a, const auto& b)
                {
                    return a.second < b.second;
                })->second;

            outLengths[agentId] = minLen;

            for (const auto& [f, len] : reachable)
            {
                if (len == minLen)
                {
                    step2[agentId].push_back(f);
                }
            }
        }

        return step2;
    }

    Assignment SelectByMaxArea(const CandidateMap& candidates)
    {
        if (candidates.empty())
        {
            return {};
        }

        std::vector<size_t> agentIds;
        agentIds.reserve(candidates.size());
        for (const auto& id : candidates | std::views::keys)
        {
            agentIds.push_back(id);
        }
        const size_t n = agentIds.size();

        const bool hasMultiple = std::any_of(candidates.begin(), candidates.end(),
            [](const auto& kv) { return kv.second.size() > 1; });

        if (!hasMultiple)
        {
            Assignment result;
            for (const auto& [id, cands] : candidates)
            {
                result[id] = cands[0];
            }
            return result;
        }

        std::vector<size_t> indices(n, 0);
        double bestArea = -1.0;
        size_t bestDistinctCount = 0;
        Assignment bestAssignment;

        while (true)
        {
            std::vector<Point> combination;
            combination.reserve(n);
            for (size_t i = 0; i < n; ++i)
            {
                combination.push_back(candidates.at(agentIds[i])[indices[i]]);
            }

            const double area = PolygonArea(combination);

            size_t distinctCount = 0;
            for (size_t i = 0; i < n; ++i)
            {
                bool isDuplicate = false;
                for (size_t j = 0; j < i; ++j)
                {
                    if (combination[i] == combination[j])
                    {
                        isDuplicate = true;
                        break;
                    }
                }
                if (!isDuplicate)
                {
                    ++distinctCount;
                }
            }

            const bool isBetter = area > bestArea
                || (area == bestArea && distinctCount > bestDistinctCount);

            if (isBetter)
            {
                bestArea = area;
                bestDistinctCount = distinctCount;
                bestAssignment.clear();
                for (size_t i = 0; i < n; ++i)
                {
                    bestAssignment[agentIds[i]] = combination[i];
                }
            }

            bool exhausted = false;
            for (size_t i = n; i-- > 0;)
            {
                ++indices[i];
                if (indices[i] < candidates.at(agentIds[i]).size())
                {
                    break;
                }
                indices[i] = 0;
                if (i == 0)
                {
                    exhausted = true;
                    break;
                }
            }
            if (exhausted)
            {
                break;
            }
        }

        return bestAssignment;
    }

    Assignment PruneClusters(Assignment assignments,
                             const PathLengthMap& pathLengths,
                             const PositionMap& currentPositions,
                             const size_t clusteringDistance)
    {
        std::vector<size_t> agentIds;
        agentIds.reserve(assignments.size());
        for (const auto& id : assignments | std::views::keys)
        {
            agentIds.push_back(id);
        }
        const size_t n = agentIds.size();

        std::unordered_map<size_t, std::vector<size_t>> adjacency;
        for (size_t i = 0; i < n; ++i)
        {
            for (size_t j = i + 1; j < n; ++j)
            {
                const size_t dist = ManhattanDistance(
                    assignments.at(agentIds[i]),
                    assignments.at(agentIds[j]));

                if (dist < clusteringDistance)
                {
                    const auto posI = currentPositions.find(agentIds[i]);
                    const auto posJ = currentPositions.find(agentIds[j]);
                    if (posI != currentPositions.end()
                        && posJ != currentPositions.end()
                        && posI->second == posJ->second)
                    {
                        continue;
                    }
                    adjacency[agentIds[i]].push_back(agentIds[j]);
                    adjacency[agentIds[j]].push_back(agentIds[i]);
                }
            }
        }

        std::unordered_set<size_t> visited;
        std::vector<size_t> toRemove;

        for (const size_t startId : agentIds)
        {
            if (visited.contains(startId))
            {
                continue;
            }

            std::vector<size_t> cluster;
            std::queue<size_t> bfsQueue;
            bfsQueue.push(startId);
            visited.insert(startId);

            while (!bfsQueue.empty())
            {
                const size_t current = bfsQueue.front();
                bfsQueue.pop();
                cluster.push_back(current);

                for (const size_t neighbor : adjacency[current])
                {
                    if (!visited.contains(neighbor))
                    {
                        visited.insert(neighbor);
                        bfsQueue.push(neighbor);
                    }
                }
            }

            if (cluster.size() <= 1)
            {
                continue;
            }

            const size_t winner = *std::min_element(cluster.begin(), cluster.end(),
                [&](const size_t a, const size_t b)
                {
                    const size_t lenA = pathLengths.count(a)
                        ? pathLengths.at(a)
                        : SIZE_MAX;

                    const size_t lenB = pathLengths.count(b)
                        ? pathLengths.at(b)
                        : SIZE_MAX;

                    return lenA != lenB ? lenA < lenB : a < b;
                });

            for (const size_t agentId : cluster)
            {
                if (agentId != winner)
                {
                    toRemove.push_back(agentId);
                }
            }
        }

        for (const size_t agentId : toRemove)
        {
            assignments.erase(agentId);
        }

        return assignments;
    }
}

Coordinator::Coordinator(const size_t id, const Point& dimensions, const Cell& positionCell,
                         const double perceptionRadius, AgentContext& context, DataBus& dataBus)
    : Agent(id, dimensions, positionCell, perceptionRadius, context, dataBus)
    , m_globalBeliefMap(dimensions)
{
}

void Coordinator::SynchronizeGlobalMap()
{
    for (const Grid* lbm : m_dataBus.GetPendingLbms() | std::views::values)
    {
        m_globalBeliefMap.MergeFrom(*lbm);
    }
    m_frontiers = Frontier::ComputeFrontiers(m_globalBeliefMap);
    m_dataBus.BroadcastGbm(m_globalBeliefMap);
}

void Coordinator::AssignTargets()
{
    if (m_frontiers.empty())
    {
        return;
    }

    PositionMap positions;
    for (const auto& agentId : m_dataBus.GetPendingLbms() | std::views::keys)
    {
        if (const Agent* agent = m_context.TryGetAgent(agentId); agent != nullptr)
        {
            positions[agentId] = agent->GetPosition().position;
        }
    }

    if (positions.empty())
    {
        return;
    }

    const CandidateMap step1 = FilterByManhattanDistance(m_frontiers, positions);

    PathLengthMap pathLengths;
    const CandidateMap step2 = FilterByPathLength(step1, positions, m_globalBeliefMap, pathLengths);

    if (step2.empty())
    {
        return;
    }

    const Assignment step3 = SelectByMaxArea(step2);
    const Assignment step4 = PruneClusters(
        step3, pathLengths, positions, static_cast<size_t>(2.0 * GetPerceptionRadius()));

    for (const auto& [agentId, target] : step4)
    {
        m_dataBus.SubmitTarget(agentId, target);
    }
}

const Grid& Coordinator::GetGlobalBeliefMap() const noexcept
{
    return m_globalBeliefMap;
}

const std::vector<Point>& Coordinator::GetFrontiers() const noexcept
{
    return m_frontiers;
}
