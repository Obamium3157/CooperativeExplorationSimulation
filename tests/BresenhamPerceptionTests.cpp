#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <cmath>
#include <set>

#include "../src/environment/Cell.h"
#include "../src/environment/CellState.h"
#include "../src/environment/Grid.h"
#include "../src/environment/Perception.h"
#include "../src/environment/Point.h"

namespace
{
    struct PointLess
    {
        bool operator()(const Point& a, const Point& b) const
        {
            return a.x < b.x || (a.x == b.x && a.y < b.y);
        }
    };


    Grid BuildGrid(const std::vector<std::string>& rows)
    {
        GridMatrix matrix;
        for (size_t y = 0; y < rows.size(); ++y)
        {
            std::vector<Cell> row;
            for (size_t x = 0; x < rows[y].size(); ++x)
            {
                CellState state;
                switch (rows[y][x])
                {
                case '#':
                    state = CellState::Obstacle;
                    break;
                case '.':
                    state = CellState::Free;
                    break;
                default:
                    state = CellState::Unknown;
                    break;
                }
                row.push_back(Cell{Point{x, y}, state});
            }
            matrix.push_back(std::move(row));
        }
        return Grid(std::move(matrix));
    }

    Grid BuildFreeGrid(const size_t width, const size_t height)
    {
        GridMatrix matrix;
        for (size_t y = 0; y < height; ++y)
        {
            std::vector<Cell> row;
            for (size_t x = 0; x < width; ++x)
            {
                row.push_back(Cell{Point{x, y}, CellState::Free});
            }
            matrix.push_back(std::move(row));
        }
        return Grid(std::move(matrix));
    }

    using PointSet = std::set<Point, PointLess>;

    PointSet ToPointSet(const std::vector<Cell>& cells)
    {
        PointSet result;
        for (const Cell& cell : cells)
        {
            result.insert(cell.position);
        }
        return result;
    }

    bool ContainsPoint(const std::vector<Cell>& cells, const size_t x, const size_t y)
    {
        return std::any_of(cells.begin(), cells.end(), [x, y](const Cell& cell)
        {
            return cell.position.x == x && cell.position.y == y;
        });
    }
}

TEST_CASE("Agent always sees its own cell (from == to)", "[HasLineOfSight]")
{
    const Grid grid = BuildFreeGrid(5, 5);
    const Point position{2, 2};

    CHECK(Perception::HasLineOfSight(position, position, grid));
}

TEST_CASE("Straight paths without obstacles are passable", "[HasLineOfSight]")
{
    const Grid grid = BuildFreeGrid(10, 10);

    SECTION("Horizontal path")
    {
        CHECK(Perception::HasLineOfSight({1, 5}, {8, 5}, grid));
    }

    SECTION("Vertical path")
    {
        CHECK(Perception::HasLineOfSight({5, 1}, {5, 8}, grid));
    }

    SECTION("Diagonal path (dx == dy)")
    {
        CHECK(Perception::HasLineOfSight({1, 1}, {7, 7}, grid));
    }

    SECTION("Inclined path (dx != dy)")
    {
        CHECK(Perception::HasLineOfSight({0, 0}, {6, 2}, grid));
    }
}

TEST_CASE("Obstacle on the path blocks visibility", "[HasLineOfSight]")
{
    SECTION("Horizontal path is blocked")
    {
        const Grid grid = BuildGrid({
            "..........",
            "....#.....",
            ".........."
        });

        CHECK_FALSE(Perception::HasLineOfSight({1, 1}, {8, 1}, grid));
    }

    SECTION("Vertical path is blocked")
    {
        const Grid grid = BuildGrid({
            "..........",
            "..........",
            "..........",
            "..........",
            ".....#....",
            "..........",
            "..........",
            "..........",
            "..........",
            ".........."
        });

        CHECK_FALSE(Perception::HasLineOfSight({5, 1}, {5, 8}, grid));
    }

    SECTION("Diagonal path is blocked")
    {
        const Grid grid = BuildGrid({
            "......",
            "......",
            "......",
            "...#..",
            "......",
            "......"
        });

        CHECK_FALSE(Perception::HasLineOfSight({1, 1}, {5, 5}, grid));
    }

    SECTION("Inclined path is blocked")
    {
        const Grid grid = BuildGrid({
            ".........",
            ".........",
            "....#....",
            ".........",
            "........."
        });

        CHECK_FALSE(Perception::HasLineOfSight({0, 0}, {8, 4}, grid));
    }
}

TEST_CASE("The final obstacle point is considered visible", "[HasLineOfSight]")
{
    const Grid grid = BuildGrid({
        "..........",
        ".......#..",
        ".........."
    });
    CHECK(Perception::HasLineOfSight({1, 1}, {7, 1}, grid));
}

TEST_CASE("Obstacle near the path does not affect visibility", "[HasLineOfSight]")
{
    const Grid grid = BuildGrid({
        "....#.....",
        "..........",
        ".........."
    });
    CHECK(Perception::HasLineOfSight({1, 1}, {8, 1}, grid));
}

TEST_CASE("HasLineOfSight is symmetrical (LoS(A, B) == LoS(B, A))", "[HasLineOfSight]")
{
    SECTION("Free grid symmetry")
    {
        const Grid grid = BuildFreeGrid(10, 10);
        CHECK(Perception::HasLineOfSight({2, 3}, {7, 6}, grid) ==
            Perception::HasLineOfSight({7, 6}, {2, 3}, grid));
    }

    SECTION("Grid with obstacles symmetry")
    {
        const Grid grid = BuildGrid({
            "..........",
            "..........",
            "..........",
            ".....#....",
            "..........",
            ".........."
        });
        const bool forwardLoS = Perception::HasLineOfSight({1, 1}, {9, 5}, grid);
        const bool backwardLoS = Perception::HasLineOfSight({9, 5}, {1, 1}, grid);

        CHECK(forwardLoS == backwardLoS);
    }
}

TEST_CASE("The target is visible through the angular contact of obstacles", "[HasLineOfSight]")
{
    const Grid grid = BuildGrid({
        ".#",
        "#."
    });

    CHECK_FALSE(Perception::HasLineOfSight({0, 0}, {1, 1}, grid));
}


TEST_CASE("Zero radius => only its own cell is considered", "[GetPerceivedCells]")
{
    const Grid grid = BuildFreeGrid(10, 10);
    const auto cells = Perception::GetPerceivedCells({5, 5}, 0.0, grid);

    REQUIRE(cells.size() == 1);
    CHECK(cells[0].position.x == 5);
    CHECK(cells[0].position.y == 5);
}

TEST_CASE("Unit radius on an open field is 5 cells", "[GetPerceivedCells]")
{
    const Grid grid = BuildFreeGrid(10, 10);
    const auto cells = Perception::GetPerceivedCells({5, 5}, 1.0, grid);
    const PointSet points = ToPointSet(cells);

    REQUIRE(cells.size() == 5);
    CHECK(points.count({5, 5}));
    CHECK(points.count({4, 5}));
    CHECK(points.count({6, 5}));
    CHECK(points.count({5, 4}));
    CHECK(points.count({5, 6}));
    CHECK_FALSE(points.count({4, 4}));
    CHECK_FALSE(points.count({6, 6}));
}

TEST_CASE("Cell on the radius boundary is included", "[GetPerceivedCells]")
{
    const Grid grid = BuildFreeGrid(15, 15);
    const auto cells = Perception::GetPerceivedCells({5, 5}, 5.0, grid);
    const PointSet points = ToPointSet(cells);

    CHECK(points.count({8, 9}));
    CHECK(points.count({2, 1}));
    CHECK(points.count({10, 5}));
    CHECK(points.count({5, 0}));
}

TEST_CASE("Cells outside the radius are not included", "[GetPerceivedCells]")
{
    const Grid grid = BuildFreeGrid(15, 15);
    const auto cells = Perception::GetPerceivedCells({5, 5}, 5.0, grid);
    const PointSet points = ToPointSet(cells);

    CHECK_FALSE(points.count({10, 6}));
    CHECK_FALSE(points.count({9, 9}));
}

TEST_CASE("Shape of the perception area is circular, not square", "[GetPerceivedCells]")
{
    const Grid grid = BuildFreeGrid(20, 20);
    const auto cells = Perception::GetPerceivedCells({10, 10}, 4.0, grid);
    const PointSet points = ToPointSet(cells);

    CHECK(points.count({14, 10}));
    CHECK(points.count({6, 10}));
    CHECK(points.count({10, 14}));
    CHECK(points.count({10, 6}));

    CHECK_FALSE(points.count({14, 14}));
    CHECK_FALSE(points.count({6, 6}));
}

TEST_CASE("Number of cells perceived matches the analytical value", "[GetPerceivedCells]")
{
    const Grid grid = BuildFreeGrid(20, 20);
    const auto cells = Perception::GetPerceivedCells({10, 10}, 4.0, grid);

    CHECK(cells.size() == 49);
}

TEST_CASE("Wall in front of the agent blocks the cells behind it", "[GetPerceivedCells]")
{
    const Grid grid = BuildGrid({
        "........",
        "........",
        ".....#..",
        ".....#..",
        ".....#..",
        "........",
        "........",
        "........"
    });
    const auto cells = Perception::GetPerceivedCells({2, 3}, 4.0, grid);
    const PointSet points = ToPointSet(cells);

    CHECK(points.count({5, 3}));

    CHECK_FALSE(points.count({6, 3}));
    CHECK_FALSE(points.count({7, 3}));

    CHECK(points.count({3, 3}));
    CHECK(points.count({4, 3}));
}

TEST_CASE("Single obstacle is perceived", "[GetPerceivedCells]")
{
    const Grid grid = BuildGrid({
        "..........",
        "..........",
        ".....#....",
        ".........."
    });
    const auto cells = Perception::GetPerceivedCells({2, 2}, 5.0, grid);

    CHECK(ContainsPoint(cells, 5, 2));
}

TEST_CASE("Obstacle creates a shadow for the cells behind it diagonally", "[GetPerceivedCells]")
{
    const Grid grid = BuildGrid({
        ".........",
        ".........",
        ".........",
        ".........",
        "....#....",
        ".........",
        ".........",
        ".........",
        "........."
    });

    const auto cells = Perception::GetPerceivedCells({1, 1}, 8.0, grid);
    const PointSet points = ToPointSet(cells);

    CHECK(points.count({4, 4}));
    CHECK_FALSE(points.count({5, 5}));
    CHECK_FALSE(points.count({6, 6}));
    CHECK(points.count({4, 3}));
    CHECK(points.count({3, 4}));
}

TEST_CASE("Agent at the edge of the grid ", "[GetPerceivedCells]")
{
    const Grid grid = BuildFreeGrid(10, 10);
    const auto cells = Perception::GetPerceivedCells({0, 0}, 5.0, grid);

    for (const Cell& cell : cells)
    {
        CHECK(cell.position.x < 10);
        CHECK(cell.position.y < 10);
    }

    CHECK(ContainsPoint(cells, 0, 0));
    CHECK_FALSE(cells.empty());
}

TEST_CASE("Agent at the bottom right edge of the grid", "[GetPerceivedCells]")
{
    const Grid grid = BuildFreeGrid(10, 10);
    const auto cells = Perception::GetPerceivedCells({9, 9}, 5.0, grid);

    for (const Cell& cell : cells)
    {
        CHECK(cell.position.x < 10);
        CHECK(cell.position.y < 10);
    }
    CHECK(ContainsPoint(cells, 9, 9));
}

TEST_CASE("Result does not contains duplicate cells", "[GetPerceivedCells]")
{
    const Grid grid = BuildFreeGrid(15, 15);
    const auto cells = Perception::GetPerceivedCells({7, 7}, 5.0, grid);
    const PointSet unique = ToPointSet(cells);

    CHECK(cells.size() == unique.size());
}

TEST_CASE("States of the returned cells correspond to realGrid", "[GetPerceivedCells]")
{
    const Grid grid = BuildGrid({
        "..........",
        "..........",
        "....#.....",
        ".........."
    });

    const auto cells = Perception::GetPerceivedCells({2, 1}, 5.0, grid);

    for (const Cell& cell : cells)
    {
        CHECK(cell.state == grid.GetCell(cell.position).state);
    }
}

TEST_CASE("All returned cells are within the given radius", "[GetPerceivedCells]")
{
    const Grid grid = BuildFreeGrid(15, 15);
    const Point position{7, 7};
    constexpr double radius = 5.0;

    const auto cells = Perception::GetPerceivedCells(position, radius, grid);

    for (const Cell& cell : cells)
    {
        const double dx = static_cast<double>(cell.position.x) - static_cast<double>(position.x);
        const double dy = static_cast<double>(cell.position.y) - static_cast<double>(position.y);
        CHECK(std::hypot(dx, dy) <= radius);
    }
}
