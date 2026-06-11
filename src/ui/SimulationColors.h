#ifndef COOPERATIVEEXPLORATIONSIMULATION_SIMULATIONCOLORS_H
#define COOPERATIVEEXPLORATIONSIMULATION_SIMULATIONCOLORS_H
#include <SFML/Graphics/Color.hpp>

namespace SimulationColors
{
    constexpr auto UnknownColor = sf::Color(80, 80, 80);
    constexpr auto FreeColor = sf::Color(220, 220, 220);
    constexpr auto ObstacleColor = sf::Color(30, 30, 30);
    constexpr auto AgentColor = sf::Color(220, 220, 200);
}

#endif //COOPERATIVEEXPLORATIONSIMULATION_SIMULATIONCOLORS_H
