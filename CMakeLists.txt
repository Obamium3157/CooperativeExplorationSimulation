cmake_minimum_required(VERSION 4.2)
project(CooperativeExplorationSimulation)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.7.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(CooperativeExplorationSimulation src/main.cpp
        src/environment/CellState.h
        src/environment/Cell.h
        src/environment/Grid.h
        src/environment/Grid.cpp
        src/ui/IDrawable.h
        src/ui/ConsoleDrawer.h
        src/ui/ConsoleDrawer.cpp
        src/ui/GUIDrawer.cpp
        src/ui/GUIDrawer.h
        src/simulation/Simulation.cpp
        src/simulation/Simulation.h
        src/ui/DrawableCharacters.h
        src/exceptions/ReadGridException.h
        src/agent/Agent.cpp
        src/agent/Agent.h
        src/agent/AgentContext.cpp
        src/agent/AgentContext.h
        src/exceptions/AgentInitializationException.h
        src/environment/Point.h
        src/exceptions/CoordinatorAssignationException.h
        src/agent/Coordinator.cpp
        src/agent/Coordinator.h
        src/environment/Perception.h
        src/environment/Perception.cpp
        src/agent/DataBus.cpp
        src/agent/DataBus.h
        src/environment/Frontier.cpp
        src/environment/Frontier.h
        src/environment/Pathfinding.cpp
        src/environment/Pathfinding.h
)

add_executable(BresenhamPerceptionTests tests/BresenhamPerceptionTests.cpp
    src/environment/Grid.cpp
    src/environment/Perception.cpp)

add_executable(MergeTests
        tests/MergeTests.cpp
        src/environment/Grid.cpp
        src/environment/Perception.cpp
        src/agent/DataBus.cpp
        src/agent/Agent.cpp
        src/agent/Coordinator.cpp
        src/agent/AgentContext.cpp)

add_executable(FrontierTests
        tests/FrontierTests.cpp
        src/environment/Grid.cpp
        src/environment/Perception.cpp
        src/environment/Frontier.cpp
        src/agent/DataBus.cpp
        src/agent/Agent.cpp
        src/agent/Coordinator.cpp
        src/agent/AgentContext.cpp)

add_executable(PathfindingTests
        tests/PathfindingTests.cpp
        src/environment/Grid.cpp
        src/environment/Frontier.cpp
        src/environment/Pathfinding.cpp)

add_executable(DataBusTests
    tests/DataBusTests.cpp
    src/agent/DataBus.h
    src/agent/DataBus.cpp
    src/environment/Grid.h
    src/environment/Grid.cpp)

target_link_libraries(BresenhamPerceptionTests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(MergeTests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(FrontierTests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(PathfindingTests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(DataBusTests PRIVATE Catch2::Catch2WithMain)