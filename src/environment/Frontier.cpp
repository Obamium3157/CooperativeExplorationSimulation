#include "Frontier.h"

#include <algorithm>

namespace Frontier
{
    std::vector<Point> OrthogonalNeighbors(const Point& point, const Point& dimensions)
    {
        std::vector<Point> neighbors;
        neighbors.reserve(4);

        const int x = static_cast<int>(point.x);
        const int y = static_cast<int>(point.y);
        const int width = static_cast<int>(dimensions.x);
        const int height = static_cast<int>(dimensions.y);

        for (const auto [dx, dy] : {
                 std::pair{0, -1}, std::pair{0, 1},
                 std::pair{-1, 0}, std::pair{1, 0}
             })
        {
            const int nx = x + dx;
            const int ny = y + dy;

            if (nx >= 0 && nx < width && ny >= 0 && ny < height)
            {
                neighbors.push_back(Point{static_cast<size_t>(nx), static_cast<size_t>(ny)});
            }
        }
        return neighbors;
    }

    bool IsFrontierCell(const Point& point, const Grid& beliefMap)
    {
        if (beliefMap.GetCell(point).state != CellState::Free)
        {
            return false;
        }

        const auto neighbors = OrthogonalNeighbors(point, beliefMap.GetDimensions());
        return std::any_of(neighbors.begin(), neighbors.end(), [&beliefMap] (const Point& neighbor)
        {
            return beliefMap.GetCell(neighbor).state == CellState::Unknown;
        });
     }

    std::vector<Point> ComputeFrontiers(const Grid& beliefMap)
    {
        std::vector<Point> frontiers;
        const auto [width, height] = beliefMap.GetDimensions();

        for (size_t y = 0; y < height; ++y)
        {
            for (size_t x = 0; x < width; ++x)
            {
                const Point point{x, y};

                if (IsFrontierCell(point, beliefMap))
                {
                    frontiers.push_back(point);
                }
            }
        }
        return frontiers;
    }
}
