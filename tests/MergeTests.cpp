#include <catch2/catch_test_macros.hpp>

#include "../src/agent/AgentContext.h"
#include "../src/agent/DataBus.h"
#include "../src/environment/Grid.h"

namespace
{

Grid MakeUniformGrid(const size_t width, const size_t height, const CellState state)
{
    GridMatrix matrix;
    for (size_t y = 0; y < height; ++y)
    {
        std::vector<Cell> row;
        for (size_t x = 0; x < width; ++x)
        {
            row.push_back(Cell{Point{x, y}, state});
        }
        matrix.push_back(std::move(row));
    }
    return Grid(std::move(matrix));
}

Grid MakeFreeGrid(const size_t width, const size_t height)
{
    return MakeUniformGrid(width, height, CellState::Free);
}

Grid MakeUnknownGrid(const size_t width, const size_t height)
{
    return MakeUniformGrid(width, height, CellState::Unknown);
}

CellState StateAt(const Grid& grid, const size_t x, const size_t y)
{
    return grid.GetCell(Point{x, y}).state;
}

}

TEST_CASE("Readiness = false until all LBMs are submitted.", "[DataBus]")
{
    DataBus bus{2};
    const Grid lbm = MakeFreeGrid(5, 5);

    CHECK_FALSE(bus.AreAllLbmsReady());

    bus.SubmitLbm(0, lbm);
    CHECK_FALSE(bus.AreAllLbmsReady());

    bus.SubmitLbm(1, lbm);
    CHECK(bus.AreAllLbmsReady());
}

TEST_CASE("Re-submitting LBM to the same agent does not increase the counter", "[DataBus]")
{
    DataBus bus{2};
    const Grid lbm = MakeFreeGrid(5, 5);

    bus.SubmitLbm(0, lbm);
    bus.SubmitLbm(0, lbm);

    CHECK_FALSE(bus.AreAllLbmsReady());
}

TEST_CASE("After BroadcastGbm, the GetGbm method returns the transmitted map", "[DataBus]")
{
    DataBus bus{1};
    Grid gbm = MakeFreeGrid(5, 5);
    gbm.SetCell(Point{2, 2}, Cell{Point{2, 2}, CellState::Obstacle});

    bus.BroadcastGbm(gbm);

    REQUIRE(bus.GetGbm().has_value());
    CHECK(StateAt(*bus.GetGbm(), 2, 2) == CellState::Obstacle);
    CHECK(StateAt(*bus.GetGbm(), 0, 0) == CellState::Free);
}

TEST_CASE("Reset resets LBM and GBM", "[DataBus]")
{
    DataBus bus{1};
    const Grid lbm = MakeFreeGrid(5, 5);

    bus.SubmitLbm(0, lbm);
    bus.BroadcastGbm(lbm);

    bus.Reset();

    CHECK_FALSE(bus.AreAllLbmsReady());
    CHECK_FALSE(bus.GetGbm().has_value());
}

TEST_CASE("Unknown source does not change target", "[MergeFrom]")
{
    const Grid source = MakeUnknownGrid(3, 3);

    SECTION("The target is Unknown => remains Unknown")
    {
        Grid target = MakeUnknownGrid(3, 3);
        target.MergeFrom(source);
        CHECK(StateAt(target, 1, 1) == CellState::Unknown);
    }
    SECTION("The target is Free => remains Free")
    {
        Grid target = MakeFreeGrid(3, 3);
        target.MergeFrom(source);
        CHECK(StateAt(target, 1, 1) == CellState::Free);
    }
    SECTION("The target is Obstacle => remains Obstacle")
    {
        Grid target = MakeUniformGrid(3, 3, CellState::Obstacle);
        target.MergeFrom(source);
        CHECK(StateAt(target, 1, 1) == CellState::Obstacle);
    }
}

TEST_CASE("Free source is written to target", "[MergeFrom]")
{
    const Grid source = MakeFreeGrid(3, 3);

    SECTION("Unknown => Free")
    {
        Grid target = MakeUnknownGrid(3, 3);
        target.MergeFrom(source);
        CHECK(StateAt(target, 1, 1) == CellState::Free);
    }
    SECTION("Free => Free")
    {
        Grid target = MakeFreeGrid(3, 3);
        target.MergeFrom(source);
        CHECK(StateAt(target, 1, 1) == CellState::Free);
    }
}

TEST_CASE("Obstacle source is written to target", "[MergeFrom]")
{
    const Grid source = MakeUniformGrid(3, 3, CellState::Obstacle);

    SECTION("Unknown => Obstacle")
    {
        Grid target = MakeUnknownGrid(3, 3);
        target.MergeFrom(source);
        CHECK(StateAt(target, 1, 1) == CellState::Obstacle);
    }
    SECTION("Obstacle => Obstacle")
    {
        Grid target = MakeUniformGrid(3, 3, CellState::Obstacle);
        target.MergeFrom(source);
        CHECK(StateAt(target, 1, 1) == CellState::Obstacle);
    }
}

TEST_CASE("Only cells with a known state in the source are changed", "[MergeFrom]")
{
    Grid target = MakeUnknownGrid(3, 3);
    Grid source = MakeUnknownGrid(3, 3);
    source.SetCell(Point{1, 1}, Cell{Point{1, 1}, CellState::Free});

    target.MergeFrom(source);

    CHECK(StateAt(target, 1, 1) == CellState::Free);
    CHECK(StateAt(target, 0, 0) == CellState::Unknown);
    CHECK(StateAt(target, 2, 2) == CellState::Unknown);
}

TEST_CASE("Merging with itself is idempotent", "[MergeFrom]")
{
    Grid grid = MakeFreeGrid(3, 3);
    grid.SetCell(Point{1, 1}, Cell{Point{1, 1}, CellState::Obstacle});

    grid.MergeFrom(grid);

    CHECK(StateAt(grid, 1, 1) == CellState::Obstacle);
    CHECK(StateAt(grid, 0, 0) == CellState::Free);
}

TEST_CASE("Completely Unknown source does not change any target cell", "[MergeFrom:]")
{
    Grid target = MakeFreeGrid(5, 5);
    target.SetCell(Point{2, 2}, Cell{Point{2, 2}, CellState::Obstacle});

    const Grid source = MakeUnknownGrid(5, 5);
    target.MergeFrom(source);

    CHECK(StateAt(target, 0, 0) == CellState::Free);
    CHECK(StateAt(target, 2, 2) == CellState::Obstacle);
}

TEST_CASE("GBM contains cells from the perceptual field of all agents", "[SynchronizeGlobalMap]")
{
    const Grid realMap = MakeFreeGrid(25, 25);
    AgentContext context{realMap, {{2, 2}, {22, 22}}, 1};

    context.IterateOverAgents();

    const Grid& gbm = context.GetCoordinator()->GetGlobalBeliefMap();

    CHECK(StateAt(gbm, 2, 2) == CellState::Free);
    CHECK(StateAt(gbm, 6, 2) == CellState::Free);

    CHECK(StateAt(gbm, 22, 22) == CellState::Free);
    CHECK(StateAt(gbm, 18, 22) == CellState::Free);

    CHECK(StateAt(gbm, 12, 12) == CellState::Unknown);
}

TEST_CASE("Agent sees only his own => the other does not see the same", "[SynchronizeGlobalMap]")
{
    const Grid realMap = MakeFreeGrid(25, 25);
    AgentContext context{realMap, {{1, 1}, {23, 23}}, 1};

    context.IterateOverAgents();

    const Grid& gbm = context.GetCoordinator()->GetGlobalBeliefMap();

    CHECK(StateAt(gbm, 1, 1) == CellState::Free);

    CHECK(StateAt(gbm, 23, 23) == CellState::Free);
}

TEST_CASE("GBM accumulates knowledge between rounds", "[SynchronizeGlobalMap]")
{
    const Grid realMap = MakeFreeGrid(25, 25);
    AgentContext context{realMap, {{2, 2}, {22, 22}}, 1};

    context.IterateOverAgents();
    context.IterateOverAgents();

    const Grid& gbm = context.GetCoordinator()->GetGlobalBeliefMap();

    CHECK(StateAt(gbm, 2, 2)  == CellState::Free);
    CHECK(StateAt(gbm, 22, 22) == CellState::Free);
    CHECK(StateAt(gbm, 12, 12) == CellState::Unknown);
}

TEST_CASE("The agent drains GBM from the bus into the local map", "[ApplyGbm]")
{
    DataBus bus{0};

    Grid gbm = MakeUnknownGrid(5, 5);
    gbm.SetCell(Point{4, 4}, Cell{Point{4, 4}, CellState::Free});
    gbm.SetCell(Point{0, 0}, Cell{Point{0, 0}, CellState::Obstacle});
    bus.BroadcastGbm(gbm);

    Grid localMap = MakeUnknownGrid(5, 5);
    localMap.MergeFrom(*bus.GetGbm());

    CHECK(StateAt(localMap, 4, 4) == CellState::Free);
    CHECK(StateAt(localMap, 0, 0) == CellState::Obstacle);
    CHECK(StateAt(localMap, 2, 2) == CellState::Unknown);
}