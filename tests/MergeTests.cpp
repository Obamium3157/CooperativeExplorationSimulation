#include <catch2/catch_test_macros.hpp>

#include "../src/agent/AgentContext.h"
#include "../src/agent/DataBus.h"
#include "../src/environment/Grid.h"

namespace
{

Grid makeUniformGrid(const size_t width, const size_t height, const CellState state)
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

Grid makeFreeGrid(const size_t width, const size_t height)
{
    return makeUniformGrid(width, height, CellState::Free);
}

Grid makeUnknownGrid(const size_t width, const size_t height)
{
    return makeUniformGrid(width, height, CellState::Unknown);
}

CellState stateAt(const Grid& grid, const size_t x, const size_t y)
{
    return grid.GetCell(Point{x, y}).state;
}

}

TEST_CASE("Readiness = false until all LBMs are submitted.", "[DataBus]")
{
    DataBus bus{2};
    const Grid lbm = makeFreeGrid(5, 5);

    CHECK_FALSE(bus.AreAllLbmsReady());

    bus.SubmitLbm(0, lbm);
    CHECK_FALSE(bus.AreAllLbmsReady());

    bus.SubmitLbm(1, lbm);
    CHECK(bus.AreAllLbmsReady());
}

TEST_CASE("Re-submitting LBM to the same agent does not increase the counter", "[DataBus]")
{
    DataBus bus{2};
    const Grid lbm = makeFreeGrid(5, 5);

    bus.SubmitLbm(0, lbm);
    bus.SubmitLbm(0, lbm);

    CHECK_FALSE(bus.AreAllLbmsReady());
}

TEST_CASE("After BroadcastGbm, the GetGbm method returns the transmitted map", "[DataBus]")
{
    DataBus bus{1};
    Grid gbm = makeFreeGrid(5, 5);
    gbm.SetCell(Point{2, 2}, Cell{Point{2, 2}, CellState::Obstacle});

    bus.BroadcastGbm(gbm);

    REQUIRE(bus.GetGbm().has_value());
    CHECK(stateAt(*bus.GetGbm(), 2, 2) == CellState::Obstacle);
    CHECK(stateAt(*bus.GetGbm(), 0, 0) == CellState::Free);
}

TEST_CASE("Reset resets LBM and GBM", "[DataBus]")
{
    DataBus bus{1};
    const Grid lbm = makeFreeGrid(5, 5);

    bus.SubmitLbm(0, lbm);
    bus.BroadcastGbm(lbm);

    bus.Reset();

    CHECK_FALSE(bus.AreAllLbmsReady());
    CHECK_FALSE(bus.GetGbm().has_value());
}

TEST_CASE("Unknown source does not change target", "[MergeFrom]")
{
    const Grid source = makeUnknownGrid(3, 3);

    SECTION("The target is Unknown => remains Unknown")
    {
        Grid target = makeUnknownGrid(3, 3);
        target.MergeFrom(source);
        CHECK(stateAt(target, 1, 1) == CellState::Unknown);
    }
    SECTION("The target is Free => remains Free")
    {
        Grid target = makeFreeGrid(3, 3);
        target.MergeFrom(source);
        CHECK(stateAt(target, 1, 1) == CellState::Free);
    }
    SECTION("The target is Obstacle => remains Obstacle")
    {
        Grid target = makeUniformGrid(3, 3, CellState::Obstacle);
        target.MergeFrom(source);
        CHECK(stateAt(target, 1, 1) == CellState::Obstacle);
    }
}

TEST_CASE("Free source is written to target", "[MergeFrom]")
{
    const Grid source = makeFreeGrid(3, 3);

    SECTION("Unknown => Free")
    {
        Grid target = makeUnknownGrid(3, 3);
        target.MergeFrom(source);
        CHECK(stateAt(target, 1, 1) == CellState::Free);
    }
    SECTION("Free => Free")
    {
        Grid target = makeFreeGrid(3, 3);
        target.MergeFrom(source);
        CHECK(stateAt(target, 1, 1) == CellState::Free);
    }
}

TEST_CASE("Obstacle source is written to target", "[MergeFrom]")
{
    const Grid source = makeUniformGrid(3, 3, CellState::Obstacle);

    SECTION("Unknown => Obstacle")
    {
        Grid target = makeUnknownGrid(3, 3);
        target.MergeFrom(source);
        CHECK(stateAt(target, 1, 1) == CellState::Obstacle);
    }
    SECTION("Obstacle => Obstacle")
    {
        Grid target = makeUniformGrid(3, 3, CellState::Obstacle);
        target.MergeFrom(source);
        CHECK(stateAt(target, 1, 1) == CellState::Obstacle);
    }
}

TEST_CASE("Only cells with a known state in the source are changed", "[MergeFrom]")
{
    Grid target = makeUnknownGrid(3, 3);
    Grid source = makeUnknownGrid(3, 3);
    source.SetCell(Point{1, 1}, Cell{Point{1, 1}, CellState::Free});

    target.MergeFrom(source);

    CHECK(stateAt(target, 1, 1) == CellState::Free);
    CHECK(stateAt(target, 0, 0) == CellState::Unknown);
    CHECK(stateAt(target, 2, 2) == CellState::Unknown);
}

TEST_CASE("Merging with itself is idempotent", "[MergeFrom]")
{
    Grid grid = makeFreeGrid(3, 3);
    grid.SetCell(Point{1, 1}, Cell{Point{1, 1}, CellState::Obstacle});

    grid.MergeFrom(grid);

    CHECK(stateAt(grid, 1, 1) == CellState::Obstacle);
    CHECK(stateAt(grid, 0, 0) == CellState::Free);
}

TEST_CASE("Completely Unknown source does not change any target cell", "[MergeFrom:]")
{
    Grid target = makeFreeGrid(5, 5);
    target.SetCell(Point{2, 2}, Cell{Point{2, 2}, CellState::Obstacle});

    const Grid source = makeUnknownGrid(5, 5);
    target.MergeFrom(source);

    CHECK(stateAt(target, 0, 0) == CellState::Free);
    CHECK(stateAt(target, 2, 2) == CellState::Obstacle);
}

TEST_CASE("GBM contains cells from the perceptual field of all agents", "[SynchronizeGlobalMap]")
{
    const Grid realMap = makeFreeGrid(25, 25);
    AgentContext context{realMap, {{2, 2}, {22, 22}}, 1};

    context.IterateOverAgents();

    const Grid& gbm = context.GetCoordinator()->GetGlobalBeliefMap();

    CHECK(stateAt(gbm, 2, 2) == CellState::Free);
    CHECK(stateAt(gbm, 6, 2) == CellState::Free);

    CHECK(stateAt(gbm, 22, 22) == CellState::Free);
    CHECK(stateAt(gbm, 18, 22) == CellState::Free);

    CHECK(stateAt(gbm, 12, 12) == CellState::Unknown);
}

TEST_CASE("Agent sees only his own => the other does not see the same", "[SynchronizeGlobalMap]")
{
    const Grid realMap = makeFreeGrid(25, 25);
    AgentContext context{realMap, {{1, 1}, {23, 23}}, 1};

    context.IterateOverAgents();

    const Grid& gbm = context.GetCoordinator()->GetGlobalBeliefMap();

    CHECK(stateAt(gbm, 1, 1) == CellState::Free);

    CHECK(stateAt(gbm, 23, 23) == CellState::Free);
}

TEST_CASE("GBM accumulates knowledge between rounds", "[SynchronizeGlobalMap]")
{
    const Grid realMap = makeFreeGrid(25, 25);
    AgentContext context{realMap, {{2, 2}, {22, 22}}, 1};

    context.IterateOverAgents();
    context.IterateOverAgents();

    const Grid& gbm = context.GetCoordinator()->GetGlobalBeliefMap();

    CHECK(stateAt(gbm, 2, 2)  == CellState::Free);
    CHECK(stateAt(gbm, 22, 22) == CellState::Free);
    CHECK(stateAt(gbm, 12, 12) == CellState::Unknown);
}

TEST_CASE("The agent drains GBM from the bus into the local map", "[ApplyGbm]")
{
    DataBus bus{0};

    Grid gbm = makeUnknownGrid(5, 5);
    gbm.SetCell(Point{4, 4}, Cell{Point{4, 4}, CellState::Free});
    gbm.SetCell(Point{0, 0}, Cell{Point{0, 0}, CellState::Obstacle});
    bus.BroadcastGbm(gbm);

    Grid localMap = makeUnknownGrid(5, 5);
    localMap.MergeFrom(*bus.GetGbm());

    CHECK(stateAt(localMap, 4, 4) == CellState::Free);
    CHECK(stateAt(localMap, 0, 0) == CellState::Obstacle);
    CHECK(stateAt(localMap, 2, 2) == CellState::Unknown);
}