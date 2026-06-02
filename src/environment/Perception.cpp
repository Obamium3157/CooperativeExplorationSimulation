#include "Perception.h"

#include <cmath>

namespace Perception
{
    bool HasLineOfSight(const Point& from, const Point& to, const Grid& grid)
    {
        int x = static_cast<int>(from.x);
        int y = static_cast<int>(from.y);

        const int targetX = static_cast<int>(to.x);
        const int targetY = static_cast<int>(to.y);

        const int dx = std::abs(targetX - x);
        const int dy = std::abs(targetY - y);
        const int stepX = x < targetX ? 1 : -1;
        const int stepY = y < targetY ? 1 : -1;
        int error = dx - dy;

        while (x != targetX || y != targetY)
        {
            const int doubleError = 2 * error;
            if (doubleError >= -dy)
            {
                error -= dy;
                x += stepX;
            }
            if (doubleError <= dx)
            {
                error += dx;
                y += stepY;
            }

            if (x == targetX && y == targetY)
            {
                break;
            }

            if (grid.GetCell(Point{static_cast<size_t>(x), static_cast<size_t>(y)}).state == CellState::Obstacle)
            {
                return false;
            }
        }

        return true;
    }

    std::vector<Cell> GetPerceivedCells(const Point& position, double radius, const Grid& realGrid)
    {
        std::vector<Cell> result;

        const auto [width, height] = realGrid.GetDimensions();
        const int cx = static_cast<int>(position.x);
        const int cy = static_cast<int>(position.y);
        const int r  = static_cast<int>(std::ceil(radius));

        const int xMin = std::max(0, cx - r);
        const int xMax = std::min(static_cast<int>(width)  - 1, cx + r);
        const int yMin = std::max(0, cy - r);
        const int yMax = std::min(static_cast<int>(height) - 1, cy + r);

        for (int y = yMin; y <= yMax; ++y)
        {
            for (int x = xMin; x <= xMax; ++x)
            {
                if (const double distance = std::hypot(x - cx, y - cy); distance > radius)
                {
                    continue;
                }

                if (const Point candidate{static_cast<size_t>(x), static_cast<size_t>(y)};
                    HasLineOfSight(position, candidate, realGrid))
                {
                    result.push_back(realGrid.GetCell(candidate));
                }
            }
        }

        return result;
    }
}
