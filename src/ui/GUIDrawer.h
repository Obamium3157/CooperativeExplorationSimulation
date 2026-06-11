#ifndef COOPERATIVEEXPLORATIONSIMULATION_GUIDRAWER_H
#define COOPERATIVEEXPLORATIONSIMULATION_GUIDRAWER_H

#include <memory>
#include <random>
#include <vector>

#include <SFML/Graphics.hpp>

#include "IDrawable.h"

class GUIDrawer : public IDrawable
{
public:
    explicit GUIDrawer(unsigned int sleepTime);

    void Draw(const Grid& grid,
              const std::vector<AgentOverlayEntry>& agentOverlay) override;

    void OnSimulationFinished() override;

private:
    void InitializeWindow(unsigned int gridWidth, unsigned int gridHeight);
    void HandleEvents() const;
    void DrawCells(const Grid& grid) const;
    void DrawAgentPath(const std::vector<Point>& path, sf::Color agentColor) const;
    void DrawAgentPaths(const std::vector<AgentOverlayEntry>& agentOverlay) const;
    void DrawAgentTarget(sf::Vector2f cellTopLeft, sf::Color agentColor) const;
    void DrawAgentTargets(const std::vector<AgentOverlayEntry>& agentOverlay) const;
    void DrawAgents(const std::vector<AgentOverlayEntry>& agentOverlay) const;
    void DrawCoordinatorMarker(sf::Vector2f cellTopLeft) const;
    void EnsureAgentColors(size_t requiredCount);
    sf::Color GenerateRandomColor();

    std::unique_ptr<sf::RenderWindow> m_window;
    std::vector<sf::Color> m_agentColors;
    std::mt19937 m_rng;
    unsigned int m_sleepTime;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_GUIDRAWER_H
