#include <random>

#include "GUIDrawer.h"

#include <thread>

#include "DrawableCharacters.h"
#include "SimulationColors.h"
#include "../environment/Grid.h"

namespace
{
    constexpr unsigned int cellSize = 20;
    constexpr unsigned int lineThickness = 2;

    sf::Vector2f CellTopLeft(const Point& point)
    {
        const float x = static_cast<float>(point.x * (cellSize + lineThickness) + lineThickness);
        const float y = static_cast<float>(point.y * (cellSize + lineThickness) + lineThickness);

        return {x, y};
    }
}

GUIDrawer::GUIDrawer(const unsigned int sleepTime)
    : m_rng(std::random_device{}())
    , m_sleepTime(sleepTime)
{
}

void GUIDrawer::Draw(const Grid& grid,
                     const std::vector<AgentOverlayEntry>& agentOverlay)
{
    const auto [width, height] = grid.GetDimensions();

    if (!m_window)
    {
        InitializeWindow(static_cast<unsigned int>(width), static_cast<unsigned int>(height));
    }

    if (!m_window->isOpen())
    {
        return;
    }

    HandleEvents();
    EnsureAgentColors(agentOverlay.size());

    m_window->clear(sf::Color::Black);
    DrawCells(grid);
    DrawAgentTargets(agentOverlay);
    DrawAgents(agentOverlay);
    m_window->display();
    std::this_thread::sleep_for(std::chrono::milliseconds(m_sleepTime));
}

void GUIDrawer::OnSimulationFinished()
{
    if (!m_window || !m_window->isOpen())
    {
        return;
    }

    while (m_window->isOpen())
    {
        while (const auto event = m_window->pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                m_window->close();
            }
        }
    }
}

void GUIDrawer::InitializeWindow(const unsigned int gridWidth, const unsigned int gridHeight)
{
    const unsigned int windowWidth = gridWidth * (cellSize + lineThickness) + lineThickness;
    const unsigned int windowHeight = gridHeight * (cellSize + lineThickness) + lineThickness;

    m_window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(sf::Vector2u{windowWidth, windowHeight}),
        "Cooperative Exploration Simulation"
    );
    m_window->setFramerateLimit(60);
}

void GUIDrawer::HandleEvents() const
{
    while (const auto event = m_window->pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            m_window->close();
        }
    }
}

void GUIDrawer::DrawCells(const Grid& grid) const
{
    const auto [width, height] = grid.GetDimensions();

    sf::RectangleShape cell(sf::Vector2f{
        static_cast<float>(cellSize),
        static_cast<float>(cellSize)
    });

    for (unsigned int y = 0; y < static_cast<unsigned int>(height); ++y)
    {
        for (unsigned int x = 0; x < static_cast<unsigned int>(width); ++x)
        {
            const Point point{x, y};

            switch (grid.GetCell(point).state)
            {
            case CellState::Unknown:
                cell.setFillColor(SimulationColors::UnknownColor);
                break;
            case CellState::Free:
                cell.setFillColor(SimulationColors::FreeColor);
                break;
            case CellState::Obstacle:
                cell.setFillColor(SimulationColors::ObstacleColor);
                break;
            case CellState::OccupiedByAgent:
                cell.setFillColor(SimulationColors::AgentColor);
                break;
            }

            cell.setPosition(CellTopLeft(point));
            m_window->draw(cell);
        }
    }
}

void GUIDrawer::DrawAgentTarget(const sf::Vector2f cellTopLeft, const sf::Color agentColor) const
{
    constexpr float markerSize = static_cast<float>(cellSize) / 2.0f;
    constexpr float markerOffset = (static_cast<float>(cellSize) - markerSize) / 2.0f;

    sf::RectangleShape marker({markerSize, markerSize});
    marker.setFillColor(agentColor);
    marker.setOutlineColor(sf::Color::Blue);
    marker.setOutlineThickness(2.0f);
    marker.setPosition({cellTopLeft.x + markerOffset, cellTopLeft.y + markerOffset});
    m_window->draw(marker);
}

void GUIDrawer::DrawAgents(const std::vector<AgentOverlayEntry>& agentOverlay) const
{
    constexpr float circleRadius = static_cast<float>(cellSize) / 2.0f - 3.0f;
    constexpr float circleOffset = static_cast<float>(cellSize) / 2.0f - circleRadius;

    for (size_t i = 0; i < agentOverlay.size(); ++i)
    {
        const sf::Vector2f topLeft = CellTopLeft(agentOverlay[i].position);

        sf::CircleShape circle(circleRadius);
        circle.setFillColor(m_agentColors[i]);
        circle.setOutlineColor(sf::Color::Black);
        circle.setOutlineThickness(2.0f);
        circle.setPosition({topLeft.x + circleOffset, topLeft.y + circleOffset});
        m_window->draw(circle);

        if (agentOverlay[i].type == DrawableCharacter::Coordinator)
        {
            DrawCoordinatorMarker(topLeft);
        }
    }
}

void GUIDrawer::DrawAgentTargets(const std::vector<AgentOverlayEntry>& agentOverlay) const
{
    for (size_t i = 0; i < agentOverlay.size(); ++i)
    {
        if (!agentOverlay[i].target.has_value())
        {
            continue;
        }
        DrawAgentTarget(CellTopLeft(*agentOverlay[i].target), m_agentColors[i]);
    }
}

void GUIDrawer::DrawCoordinatorMarker(const sf::Vector2f cellTopLeft) const
{
    sf::RectangleShape border(sf::Vector2f{
        static_cast<float>(cellSize),
        static_cast<float>(cellSize)
    });
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::Red);
    border.setOutlineThickness(-2.0f);
    border.setPosition(cellTopLeft);
    m_window->draw(border);
}

void GUIDrawer::EnsureAgentColors(const size_t requiredCount)
{
    while (m_agentColors.size() < requiredCount)
    {
        m_agentColors.push_back(GenerateRandomColor());
    }
}

sf::Color GUIDrawer::GenerateRandomColor()
{
    std::uniform_int_distribution<int> dist(80, 255);
    return sf::Color(
        static_cast<uint8_t>(dist(m_rng)),
        static_cast<uint8_t>(dist(m_rng)),
        static_cast<uint8_t>(dist(m_rng))
    );
}
