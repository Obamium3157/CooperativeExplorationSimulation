#include "Simulation.h"

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <ranges>

#include "../exceptions/ReadGridException.h"
#include "../ui/DrawableCharacters.h"
#include "../ui/ConsoleDrawer.h"
#include "../ui/GUIDrawer.h"

namespace
{
    std::unordered_map<char, CellState> charToCellStateMap = {
        {DrawableCharacter::Unknown, CellState::Unknown},
        {DrawableCharacter::Free, CellState::Free},
        {DrawableCharacter::Obstacle, CellState::Obstacle},
        {DrawableCharacter::Agent, CellState::OccupiedByAgent},
    };

    CellState GetSellStateByChar(const char ch)
    {
        const auto it = charToCellStateMap.find(ch);
        if (it == charToCellStateMap.end())
        {
            throw ReadGridException("No such character: " + std::string(1, ch));
        }

        return it->second;
    }
}

Simulation::Simulation(const std::filesystem::path& filename, const DrawableVariant variant)
    : m_drawable(MakeDrawable(variant))
    , m_map(Grid(LoadGridFromFile(filename)))
{
}

void Simulation::Run() const
{
    m_drawable->Draw(m_map);
}

GridMatrix Simulation::LoadGridFromFile(const std::filesystem::path& filename)
{
    GridMatrix matrix;

    std::ifstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + filename.string());
    }

    std::string line;
    unsigned int y = 0;
    while (std::getline(file, line))
    {
        matrix.emplace_back();
        std::istringstream lineStream(line);
        for (auto [x, ch] : std::views::enumerate(line))
        {
            if (std::isspace(ch))
            {
                continue;
            }

            auto state = GetSellStateByChar(ch);
            matrix[y].emplace_back(Cell{static_cast<unsigned int>(x), y, state});
        }
        y++;
    }

    return matrix;
}

std::unique_ptr<IDrawable> Simulation::MakeDrawable(const DrawableVariant variant)
{
    switch (variant)
    {
    case DrawableVariant::Console: return std::make_unique<ConsoleDrawer>(std::cout);
    case DrawableVariant::GUI: return std::make_unique<GUIDrawer>();
    default: return std::make_unique<ConsoleDrawer>(std::cout);
    }
}
