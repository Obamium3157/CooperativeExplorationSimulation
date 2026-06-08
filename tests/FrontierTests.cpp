#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <set>

#include "../src/agent/AgentContext.h"
#include "../src/environment/Frontier.h"
#include "../src/environment/Grid.h"

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

    PointSet ToPointSet(const std::vector<Point>& points)
    {
        PointSet result;
        for (const Point& p : points)
        {
            result.insert(p);
        }
        return result;
    }

    bool ContainsPoint(const std::vector<Point>& points, const size_t x, const size_t y)
    {
        return std::any_of(points.begin(), points.end(),
                           [x, y](const Point& p) { return p.x == x && p.y == y; });
    }
}


TEST_CASE("Inner cell has exactly 4 neighbors", "[OrthogonalNeighbors]")
{
    const Point dimensions{5, 5};
    const auto neighbors = Frontier::OrthogonalNeighbors({2, 2}, dimensions);
    const PointSet result = ToPointSet(neighbors);

    REQUIRE(neighbors.size() == 4);
    CHECK(result.count({2, 1}));
    CHECK(result.count({2, 3}));
    CHECK(result.count({1, 2}));
    CHECK(result.count({3, 2}));
}

TEST_CASE("Corner cell (0,0) has exactly 2 neighbors", "[OrthogonalNeighbors]")
{
    const Point dimensions{5, 5};
    const auto neighbors = Frontier::OrthogonalNeighbors({0, 0}, dimensions);
    const PointSet result = ToPointSet(neighbors);

    REQUIRE(neighbors.size() == 2);
    CHECK(result.count({1, 0}));
    CHECK(result.count({0, 1}));
}

TEST_CASE("Edge cell has exactly 3 neighbors", "[OrthogonalNeighbors]")
{
    const Point dimensions{5, 5};
    const auto neighbors = Frontier::OrthogonalNeighbors({0, 2}, dimensions);
    const PointSet result = ToPointSet(neighbors);

    REQUIRE(neighbors.size() == 3);
    CHECK(result.count({0, 1}));
    CHECK(result.count({0, 3}));
    CHECK(result.count({1, 2}));
}

TEST_CASE("No neighbors outside the grid bounds", "[OrthogonalNeighbors]")
{
    const Point dimensions{3, 3};
    for (const auto [x, y] : {std::pair{0u, 0u}, {2u, 0u}, {0u, 2u}, {2u, 2u}})
    {
        for (const Point& n : Frontier::OrthogonalNeighbors({x, y}, dimensions))
        {
            CHECK(n.x < 3);
            CHECK(n.y < 3);
        }
    }
}

TEST_CASE("Unknown cell is not a frontier", "[IsFrontierCell]")
{
    const Grid grid = BuildGrid({"???", "???", "???"});
    CHECK_FALSE(Frontier::IsFrontierCell({1, 1}, grid));
}

TEST_CASE("Obstacle cell is not a frontier", "[IsFrontierCell]")
{
    const Grid grid = BuildGrid({"???", "?#?", "???"});
    CHECK_FALSE(Frontier::IsFrontierCell({1, 1}, grid));
}

TEST_CASE("Free cell without Unknown neighbors is not a frontier", "[IsFrontierCell]")
{
    SECTION("All neighbors are Free")
    {
        const Grid grid = BuildGrid({"...", "...", "..."});
        CHECK_FALSE(Frontier::IsFrontierCell({1, 1}, grid));
    }
    SECTION("All neighbors are Obstacle")
    {
        const Grid grid = BuildGrid({"###", "#.#", "###"});
        CHECK_FALSE(Frontier::IsFrontierCell({1, 1}, grid));
    }
}

TEST_CASE("Free cell with at least one Unknown neighbor is a frontier", "[IsFrontierCell]")
{
    SECTION("One Unknown neighbor")
    {
        const Grid grid = BuildGrid({".?.", "...", "..."});
        CHECK(Frontier::IsFrontierCell({1, 1}, grid));
    }
    SECTION("All neighbors are Unknown")
    {
        const Grid grid = BuildGrid({"???", "?.?", "???"});
        CHECK(Frontier::IsFrontierCell({1, 1}, grid));
    }
    SECTION("Mixed neighbors: Free and Unknown")
    {
        const Grid grid = BuildGrid({".?.", "...", ".?."});
        CHECK(Frontier::IsFrontierCell({1, 1}, grid));
    }
}

TEST_CASE("Corner cell with Unknown in-bounds neighbors is a frontier", "[IsFrontierCell]")
{
    const Grid grid = BuildGrid({".?", "??"});
    CHECK(Frontier::IsFrontierCell({0, 0}, grid));
}

TEST_CASE("Corner cell with no Unknown in-bounds neighbors is not a frontier", "[IsFrontierCell]")
{
    const Grid grid = BuildGrid({"..", ".."});
    CHECK_FALSE(Frontier::IsFrontierCell({0, 0}, grid));
}

TEST_CASE("Fully Unknown map has no frontiers", "[ComputeFrontiers]")
{
    const Grid grid = BuildGrid({"???", "???", "???"});
    CHECK(Frontier::ComputeFrontiers(grid).empty());
}

TEST_CASE("Fully Free map has no frontiers", "[ComputeFrontiers]")
{
    const Grid grid = BuildGrid({"...", "...", "..."});
    CHECK(Frontier::ComputeFrontiers(grid).empty());
}

TEST_CASE("Single Free cell among Unknown cells is the only frontier", "[ComputeFrontiers]")
{
    Grid grid(Point{5, 5});
    grid.SetCell(Point{2, 2}, Cell{Point{2, 2}, CellState::Free});

    const auto frontiers = Frontier::ComputeFrontiers(grid);

    REQUIRE(frontiers.size() == 1);
    CHECK(frontiers[0].x == 2);
    CHECK(frontiers[0].y == 2);
}

TEST_CASE("Corridor: all Free cells along Unknown border are frontiers", "[ComputeFrontiers]")
{
    const Grid grid = BuildGrid({
        "?????",
        "...??",
        "?????"
    });
    const auto frontiers = Frontier::ComputeFrontiers(grid);

    CHECK(ContainsPoint(frontiers, 0, 1));
    CHECK(ContainsPoint(frontiers, 1, 1));
    CHECK(ContainsPoint(frontiers, 2, 1));
}

TEST_CASE("Fully explored zone surrounded by obstacles has no frontiers", "[ComputeFrontiers]")
{
    const Grid grid = BuildGrid({
        "#####",
        "#...#",
        "#...#",
        "#...#",
        "#####"
    });
    CHECK(Frontier::ComputeFrontiers(grid).empty());
}

TEST_CASE("No duplicate points in the result", "[ComputeFrontiers]")
{
    const Grid grid = BuildGrid({
        "???",
        "?.?",
        "???"
    });
    const auto frontiers = Frontier::ComputeFrontiers(grid);
    const PointSet unique = ToPointSet(frontiers);

    CHECK(frontiers.size() == unique.size());
}

TEST_CASE("After IterateOverAgents coordinator has non-empty frontier set", "[ComputeFrontiers]")
{
    const Grid realMap = BuildFreeGrid(20, 20);
    AgentContext context{realMap, {{2, 2}, {17, 17}}, 1};

    context.IterateOverAgents();

    CHECK_FALSE(context.GetCoordinator()->GetFrontiers().empty());
}
