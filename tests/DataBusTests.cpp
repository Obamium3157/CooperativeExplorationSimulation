#include <catch2/catch_test_macros.hpp>

#include "../src/agent/DataBus.h"
#include "../src/environment/Grid.h"

namespace
{
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

    Grid BuildUnknownGrid(const size_t width, const size_t height)
    {
        GridMatrix matrix;
        for (size_t y = 0; y < height; ++y)
        {
            std::vector<Cell> row;
            for (size_t x = 0; x < width; ++x)
            {
                row.push_back(Cell{Point{x, y}, CellState::Unknown});
            }
            matrix.push_back(std::move(row));
        }
        return Grid(std::move(matrix));
    }

    CellState StateAt(const Grid& grid, const size_t x, const size_t y)
    {
        return grid.GetCell(Point{x, y}).state;
    }
}

TEST_CASE("AreAllLbmsReady returns false until all agents have submitted", "[SubmitLbm]")
{
    DataBus bus{3};
    const Grid lbm = BuildFreeGrid(5, 5);

    CHECK_FALSE(bus.AreAllLbmsReady());

    bus.SubmitLbm(0, lbm);
    CHECK_FALSE(bus.AreAllLbmsReady());

    bus.SubmitLbm(1, lbm);
    CHECK_FALSE(bus.AreAllLbmsReady());

    bus.SubmitLbm(2, lbm);
    CHECK(bus.AreAllLbmsReady());
}

TEST_CASE("Resubmitting from the same agent overwrites the previous submission", "[SubmitLbm]")
{
    DataBus bus{2};
    const Grid lbm = BuildFreeGrid(5, 5);

    bus.SubmitLbm(0, lbm);
    bus.SubmitLbm(0, lbm);

    CHECK_FALSE(bus.AreAllLbmsReady());
}

TEST_CASE("GetPendingLbms contains the submitted map", "[SubmitLbm]")
{
    DataBus bus{1};
    Grid lbm = BuildUnknownGrid(5, 5);
    lbm.SetCell(Point{2, 3}, Cell{Point{2, 3}, CellState::Free});

    bus.SubmitLbm(0, lbm);

    const auto& pending = bus.GetPendingLbms();
    REQUIRE(pending.count(0) == 1);
    CHECK(StateAt(pending.at(0), 2, 3) == CellState::Free);
    CHECK(StateAt(pending.at(0), 0, 0) == CellState::Unknown);
}

TEST_CASE("GetGbm returns nullopt before any broadcast", "[BroadcastGbm]")
{
    DataBus bus{1};
    CHECK_FALSE(bus.GetGbm().has_value());
}

TEST_CASE("GetGbm returns the map passed to BroadcastGbm", "[BroadcastGbm]")
{
    DataBus bus{1};
    Grid gbm = BuildFreeGrid(5, 5);
    gbm.SetCell(Point{2, 2}, Cell{Point{2, 2}, CellState::Obstacle});

    bus.BroadcastGbm(gbm);

    REQUIRE(bus.GetGbm().has_value());
    CHECK(StateAt(*bus.GetGbm(), 2, 2) == CellState::Obstacle);
    CHECK(StateAt(*bus.GetGbm(), 0, 0) == CellState::Free);
}

TEST_CASE("ReceiveTarget returns the target submitted for an agent", "[SubmitTarget]")
{
    DataBus bus{1};
    bus.SubmitTarget(0, Point{3, 7});

    const auto target = bus.ReceiveTarget(0);

    REQUIRE(target.has_value());
    CHECK(target->x == 3);
    CHECK(target->y == 7);
}

TEST_CASE("Each agent receives only its own target", "[SubmitTarget]")
{
    DataBus bus{2};
    bus.SubmitTarget(0, Point{1, 2});
    bus.SubmitTarget(1, Point{9, 8});

    const auto target0 = bus.ReceiveTarget(0);
    const auto target1 = bus.ReceiveTarget(1);

    REQUIRE(target0.has_value());
    CHECK(target0->x == 1);
    CHECK(target0->y == 2);

    REQUIRE(target1.has_value());
    CHECK(target1->x == 9);
    CHECK(target1->y == 8);
}

TEST_CASE("ReceiveTarget returns nullopt for an agent without an assigned target", "[SubmitTarget]")
{
    DataBus bus{2};
    bus.SubmitTarget(0, Point{5, 5});

    CHECK_FALSE(bus.ReceiveTarget(1).has_value());
}

TEST_CASE("Reset clears pending LBMs", "[Reset]")
{
    DataBus bus{1};
    bus.SubmitLbm(0, BuildFreeGrid(5, 5));

    bus.Reset();

    CHECK_FALSE(bus.AreAllLbmsReady());
    CHECK(bus.GetPendingLbms().empty());
}

TEST_CASE("Reset clears the broadcast GBM", "[Reset]")
{
    DataBus bus{1};
    bus.BroadcastGbm(BuildFreeGrid(5, 5));

    bus.Reset();

    CHECK_FALSE(bus.GetGbm().has_value());
}

TEST_CASE("Reset clears assigned targets", "[Reset]")
{
    DataBus bus{1};
    bus.SubmitTarget(0, Point{4, 4});

    bus.Reset();

    CHECK_FALSE(bus.ReceiveTarget(0).has_value());
}