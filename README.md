## Диаграмма классов


### Модель среды

```mermaid
classDiagram
    class Point {
        +size_t x
        +size_t y
        +bool operator==(Point other)
    }

    class CellState {
        <<enumeration>>
        Unknown
        Free
        Obstacle
        OccupiedByAgent
    }

    class Cell {
        +Point position
        +CellState state
    }

    class Grid {
        -GridMatrix m_grid
        +GetCell(Point) Cell
        +SetCell(Point, Cell) void
        +GetDimensions() Point
        +GetGridMatrix() GridMatrix
        +MergeFrom(Grid) void
    }

    class Frontier {
        <<utility>>
        +ComputeFrontiers(Grid)$ vector~Point~
        +IsFrontierCell(Point, Grid)$ bool
        +OrthogonalNeighbors(Point, Point)$ vector~Point~
    }

    class Perception {
        <<utility>>
        +GetPerceivedCells(Point, double, Grid)$ vector~Cell~
        +HasLineOfSight(Point, Point, Grid)$ bool
    }

    class Pathfinding {
        <<utility>>
        +FindPath(Point, Point, Grid)$ optional~Path~
        +FindPathLength(Point, Point, Grid)$ optional~size_t~
    }

    Cell *-- Point
    Cell --> CellState
    Grid *-- Cell : GridMatrix

    Frontier ..> Grid
    Frontier ..> Point
    Perception ..> Grid
    Perception ..> Cell
    Pathfinding ..> Grid
    Pathfinding ..> Point
```

### Агентная система

```mermaid
classDiagram
    direction LR
    class DataBus {
        -size_t m_agentCount
        -map~size_t, Grid*~ m_pendingLbms
        -Grid* m_currentGbm
        -map~size_t, Point~ m_pendingTargets
        +SubmitLbm(size_t, Grid) void
        +AreAllLbmsReady() bool
        +GetPendingLbms() map
        +BroadcastGbm(Grid) void
        +GetGbm() Grid*
        +SubmitTarget(size_t, Point) void
        +ReceiveTarget(size_t) optional~Point~
        +Reset() void
    }

    class Agent {
        -size_t m_id
        -Grid m_localBeliefMap
        -Cell m_currentCell
        -vector~Point~ m_currentPath
        -double m_perceptionRadius
        -bool m_hasArrived
        +GetId() size_t
        +GetPosition() Cell
        +GetLocalBeliefMap() Grid
        +HasArrived() bool
        +Act() void
        +ApplyGbm() void
        +ReceiveAndPlanTarget() void
        +Step() void
        +Perceive() void
    }

    class Coordinator {
        -Grid m_globalBeliefMap
        -vector~Point~ m_frontiers
        +SynchronizeGlobalMap() void
        +AssignTargets() void
        +GetGlobalBeliefMap() Grid
        +GetFrontiers() vector~Point~
    }

    class AgentContext {
        <<see Fig.3>>
    }

    Coordinator --|> Agent
    Agent o-- DataBus : ref
    Agent o-- AgentContext : ref
    Coordinator ..> DataBus
```

### Управление симуляцией

```mermaid
classDiagram
    class AgentContext {
        -DataBus m_dataBus
        -map~size_t, Agent*~ m_agentById
        -Coordinator* m_coordinator
        -Grid m_map
        -size_t m_pendingAgentCount
        -size_t m_simulationTime
        +IterateOverAgents() void
        +GetCoordinator() Coordinator*
        +GetMap() Grid
        +GetSimulationTime() size_t
        +GetAgentInfos() vector~AgentInfo~
        +GetAgent(size_t) Agent*
    }

    class Simulation {
        -Grid m_map
        -unique_ptr~AgentContext~ m_context
        -unique_ptr~IDrawable~ m_drawable
        +Run() void
        -LoadGridFromFile(path) GridMatrix
        -MakeDrawable(DrawableVariant) IDrawable*
        -BuildAgentOverlay() vector
    }

    class DrawableVariant {
        <<enumeration>>
        Console
        GUI
    }

    class AgentInitializationException {
        <<exception>>
    }

    class CoordinatorAssignationException {
        <<exception>>
    }

    class ReadGridException {
        <<exception>>
    }

    class Agent {
        <<see Fig.2>>
    }

    class Coordinator {
        <<see Fig.2>>
    }

    class IDrawable {
        <<see Fig.4>>
    }

    Simulation *-- AgentContext
    Simulation --> IDrawable
    Simulation --> DrawableVariant
    AgentContext *-- DataBus
    AgentContext o-- Agent
    AgentContext --> Coordinator

    Simulation ..> AgentInitializationException : throws
    Simulation ..> ReadGridException : throws
    AgentContext ..> AgentInitializationException : throws
    AgentContext ..> CoordinatorAssignationException : throws
```

### Слой визуализации

```mermaid
classDiagram
    class IDrawable {
        <<interface>>
        +Draw(Grid, vector~pair~)* void
    }

    class ConsoleDrawer {
        -ostream m_out
        +Draw(Grid, vector~pair~) void
    }

    class GUIDrawer {
        +Draw(Grid, vector~pair~) void
    }

    class DrawableCharacters {
        <<constants>>
        +char Unknown$
        +char Free$
        +char Obstacle$
        +char Agent$
        +char Coordinator$
        +char GridCorner$
        +char GridBorderVertical$
        +char GridBorderHorizontal$
    }

    class Grid {
        <<see Fig.1>>
    }

    class Cell {
        <<see Fig.1>>
    }

    ConsoleDrawer --|> IDrawable
    GUIDrawer --|> IDrawable
    ConsoleDrawer ..> DrawableCharacters
    ConsoleDrawer ..> Grid
    ConsoleDrawer ..> Cell
    IDrawable ..> Grid
```
