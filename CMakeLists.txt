cmake_minimum_required(VERSION 4.2)
project(CooperativeExplorationSimulation)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.7.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(CooperativeExplorationSimulation src/main.cpp
        src/environment/CellState.h
        src/environment/CellState.cpp
        src/environment/Cell.h
        src/environment/Grid.h
        src/environment/Grid.cpp
        src/ui/IDrawable.h
        src/ui/ConsoleDrawer.h
        src/ui/ConsoleDrawer.cpp
        src/ui/GUIDrawer.cpp
        src/ui/GUIDrawer.h
)