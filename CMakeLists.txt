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
)

add_executable(BresenhamPerceptionTests tests/BresenhamPerceptionTests.cpp
    src/environment/Grid.cpp
    src/environment/Perception.cpp)

target_link_libraries(BresenhamPerceptionTests PRIVATE Catch2::Catch2WithMain)