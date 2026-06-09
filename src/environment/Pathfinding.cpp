#include "Pathfinding.h"

#include <algorithm>
#include <cstdint>
#include <queue>

#include "CellState.h"
#include "Frontier.h"

namespace
{
    size_t ManhattanDistance(const Point& a, const Point& b) noexcept
    {
        const size_t dx = a.x > b.x ? a.x - b.x : b.x - a.x;
        const size_t dy = a.y > b.y ? a.y - b.y : b.y - a.y;
        return dx + dy;
    }

    size_t ToFlatIndex(const Point& point, const size_t width) noexcept
    {
        return point.y * width + point.x;
    }

    bool IsTraversable(const Point& point, const Grid& beliefMap)
    {
        return beliefMap.GetCell(point).state == CellState::Free;
    }

    struct OpenNode
    {
        size_t fScore;
        Point position;

        bool operator>(const OpenNode& other) const noexcept
        {
            return fScore > other.fScore;
        }
    };

    std::vector<Point> ReconstructPath(const std::vector<size_t>& cameFrom,
                                       const size_t targetIndex,
                                       const size_t width)
    {
        std::vector<Point> path;
        size_t current = targetIndex;
        while (current != SIZE_MAX)
        {
            path.push_back(Point{current % width, current / width});
            current = cameFrom[current];
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    size_t RunAStar(const Point& from, const Point& to, const Grid& beliefMap,
                    std::vector<size_t>& gScore, std::vector<size_t>* cameFrom)
    {
        const auto [width, height] = beliefMap.GetDimensions();
        const Point dimensions{width, height};
        const size_t totalCells = width * height;

        gScore.assign(totalCells, SIZE_MAX);
        if (cameFrom != nullptr)
        {
            cameFrom->assign(totalCells, SIZE_MAX);
        }
        std::vector<bool> closed(totalCells, false);

        std::priority_queue<OpenNode, std::vector<OpenNode>, std::greater<>> openSet;
        gScore[ToFlatIndex(from, width)] = 0;
        openSet.push({ManhattanDistance(from, to), from});

        while (!openSet.empty())
        {
            const auto [f, current] = openSet.top();
            openSet.pop();

            const size_t currentIndex = ToFlatIndex(current, width);
            if (closed[currentIndex])
            {
                continue;
            }
            closed[currentIndex] = true;

            if (current.x == to.x && current.y == to.y)
            {
                return currentIndex;
            }

            for (const Point& neighbor : Frontier::OrthogonalNeighbors(current, dimensions))
            {
                if (!IsTraversable(neighbor, beliefMap))
                {
                    continue;
                }

                const size_t neighborIndex = ToFlatIndex(neighbor, width);
                if (closed[neighborIndex])
                {
                    continue;
                }

                const size_t tentativeG = gScore[currentIndex] + 1;
                if (tentativeG < gScore[neighborIndex])
                {
                    gScore[neighborIndex] = tentativeG;
                    if (cameFrom)
                    {
                        (*cameFrom)[neighborIndex] = currentIndex;
                    }
                    openSet.push({tentativeG + ManhattanDistance(neighbor, to), neighbor});
                }
            }
        }

        return SIZE_MAX;
    }
}


namespace Pathfinding
{
    std::optional<std::vector<Point>> FindPath(const Point& from, const Point& to,
                                               const Grid& beliefMap)
    {
        if (from.x == to.x && from.y == to.y)
        {
            return std::vector{from};
        }

        const size_t width = beliefMap.GetDimensions().x;
        std::vector<size_t> gScore;
        std::vector<size_t> cameFrom;

        const size_t targetIndex = RunAStar(from, to, beliefMap, gScore, &cameFrom);
        if (targetIndex == SIZE_MAX)
        {
            return std::nullopt;
        }

        return ReconstructPath(cameFrom, targetIndex, width);
    }

    std::optional<size_t> FindPathLength(const Point& from, const Point& to,
                                         const Grid& beliefMap)
    {
        if (from.x == to.x && from.y == to.y)
        {
            return 0;
        }

        std::vector<size_t> gScore;
        const size_t targetIndex = RunAStar(from, to, beliefMap, gScore, nullptr);
        if (targetIndex == SIZE_MAX)
        {
            return std::nullopt;
        }

        return gScore[targetIndex];
    }
}
