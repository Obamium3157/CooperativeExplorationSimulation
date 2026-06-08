#include <catch2/catch_test_macros.hpp>

#include <algorithm>

#include "../src/environment/Grid.h"
#include "../src/environment/Pathfinding.h"

namespace
{
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

    bool AreOrthogonallyAdjacent(const Point& a, const Point& b) noexcept
    {
        const size_t dx = a.x > b.x ? a.x - b.x : b.x - a.x;
        const size_t dy = a.y > b.y ? a.y - b.y : b.y - a.y;
        return dx + dy == 1;
    }

    bool IsValidPath(const std::vector<Point>& path)
    {
        if (path.empty())
        {
            return false;
        }
        for (size_t i = 1; i < path.size(); ++i)
        {
            if (!AreOrthogonallyAdjacent(path[i - 1], path[i]))
            {
                return false;
            }
        }
        return true;
    }

    bool AllCellsAreFree(const std::vector<Point>& path, const Grid& grid)
    {
        return std::all_of(path.begin(), path.end(), [&grid](const Point& p)
        {
            return grid.GetCell(p).state == CellState::Free;
        });
    }
}


TEST_CASE("From == to returns single-element path", "[FindPath]")
{
    const Grid grid = BuildFreeGrid(5, 5);
    const auto path = Pathfinding::FindPath({2, 2}, {2, 2}, grid);

    REQUIRE(path.has_value());
    REQUIRE(path->size() == 1);
    CHECK((*path)[0].x == 2);
    CHECK((*path)[0].y == 2);
}

TEST_CASE("Adjacent cells yield two-element path", "[FindPath]")
{
    const Grid grid = BuildFreeGrid(5, 5);
    const auto path = Pathfinding::FindPath({2, 2}, {3, 2}, grid);

    REQUIRE(path.has_value());
    REQUIRE(path->size() == 2);
    CHECK((*path)[0].x == 2);
    CHECK((*path)[0].y == 2);
    CHECK((*path)[1].x == 3);
    CHECK((*path)[1].y == 2);
}

TEST_CASE("Path starts at from and ends at to", "[FindPath]")
{
    const Grid grid = BuildFreeGrid(10, 10);
    const Point from{1, 1};
    const Point to{8, 7};
    const auto path = Pathfinding::FindPath(from, to, grid);

    REQUIRE(path.has_value());
    CHECK(path->front().x == from.x);
    CHECK(path->front().y == from.y);
    CHECK(path->back().x == to.x);
    CHECK(path->back().y == to.y);
}

TEST_CASE("All consecutive cells in the path are orthogonally adjacent", "[FindPath]")
{
    const Grid grid = BuildFreeGrid(10, 10);
    const auto path = Pathfinding::FindPath({0, 0}, {9, 9}, grid);

    REQUIRE(path.has_value());
    CHECK(IsValidPath(*path));
}

TEST_CASE("All cells in the path are Free", "[FindPath]")
{
    const Grid grid = BuildGrid({
        ".#.",
        "...",
        "..."
    });

    const auto path = Pathfinding::FindPath({0, 0}, {2, 0}, grid);

    REQUIRE(path.has_value());
    CHECK(AllCellsAreFree(*path, grid));
}

TEST_CASE("Finds the shortest path length on an open grid", "[FindPath]")
{
    const Grid grid = BuildFreeGrid(10, 10);
    const Point from{1, 1};
    const Point to{5, 4};
    const auto path = Pathfinding::FindPath(from, to, grid);

    REQUIRE(path.has_value());

    CHECK(path->size() == 8);
}

TEST_CASE("Navigates around a single obstacle", "[FindPath]")
{
    const Grid grid = BuildGrid({
        ".#.",
        "..."
    });
    const auto path = Pathfinding::FindPath({0, 0}, {2, 0}, grid);

    REQUIRE(path.has_value());
    CHECK(IsValidPath(*path));
    CHECK(AllCellsAreFree(*path, grid));
    CHECK(path->size() == 5);
}

TEST_CASE("Returns nullopt when target is completely surrounded by obstacles", "[FindPath]")
{
    const Grid grid = BuildGrid({
        ".....",
        "..#..",
        ".#.#.",
        "..#..",
        "....."
    });
    const auto path = Pathfinding::FindPath({0, 0}, {2, 2}, grid);
    CHECK_FALSE(path.has_value());
}

TEST_CASE("Returns nullopt when start and target are in disconnected components", "[FindPath]")
{
    const Grid grid = BuildGrid({
        "...#...",
        "...#...",
        "...#...",
        "...#..."
    });
    const auto path = Pathfinding::FindPath({1, 1}, {5, 1}, grid);
    CHECK_FALSE(path.has_value());
}

TEST_CASE("Finds path through a one-cell-wide corridor", "[FindPath]")
{
    const Grid grid = BuildGrid({
        "#####",
        "#...#",
        "###.#",
        "#...#",
        "#####"
    });

    const auto path = Pathfinding::FindPath({1, 1}, {1, 3}, grid);

    REQUIRE(path.has_value());
    CHECK(IsValidPath(*path));
    CHECK(AllCellsAreFree(*path, grid));
}

TEST_CASE("Unknown cells are not traversable", "[FindPath]")
{
    const Grid grid = BuildGrid({
        ".?.",
        "..."
    });

    const auto path = Pathfinding::FindPath({0, 0}, {2, 0}, grid);

    REQUIRE(path.has_value());
    CHECK(AllCellsAreFree(*path, grid));
    CHECK(path->size() == 5);
}

TEST_CASE("From == to returns zero", "[FindPathLength]")
{
    const Grid grid = BuildFreeGrid(5, 5);
    const auto length = Pathfinding::FindPathLength({3, 3}, {3, 3}, grid);

    REQUIRE(length.has_value());
    CHECK(*length == 0);
}

TEST_CASE("Adjacent cells return length 1", "[FindPathLength]")
{
    const Grid grid = BuildFreeGrid(5, 5);
    const auto length = Pathfinding::FindPathLength({2, 2}, {2, 3}, grid);

    REQUIRE(length.has_value());
    CHECK(*length == 1);
}

TEST_CASE("Equals path.size() - 1", "[FindPathLength]")
{
    const Grid grid = BuildFreeGrid(10, 10);
    const Point from{0, 0};
    const Point to{6, 4};

    const auto path = Pathfinding::FindPath(from, to, grid);
    const auto length = Pathfinding::FindPathLength(from, to, grid);

    REQUIRE(path.has_value());
    REQUIRE(length.has_value());
    CHECK(*length == path->size() - 1);
}

TEST_CASE("Returns nullopt when no path exists", "[FindPathLength]")
{
    const Grid grid = BuildGrid({
        "..#..",
        "..#..",
        "..#..",
        "..#.."
    });
    const auto length = Pathfinding::FindPathLength({0, 0}, {4, 0}, grid);
    CHECK_FALSE(length.has_value());
}

TEST_CASE("On open grid equals Manhattan distance", "[FindPathLength]")
{
    const Grid grid = BuildFreeGrid(15, 15);
    const Point from{2, 3};
    const Point to{9, 8};
    const auto length = Pathfinding::FindPathLength(from, to, grid);

    REQUIRE(length.has_value());

    const size_t expectedLength = (9 - 2) + (8 - 3);
    CHECK(*length == expectedLength);
}
