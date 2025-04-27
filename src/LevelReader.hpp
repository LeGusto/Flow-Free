#include "FlowGrid.hpp"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>

std::unordered_map<char, sf::Color> colorMap = {
    {'R', sf::Color::Red},
    {'G', sf::Color::Green},
    {'B', sf::Color::Blue},
    {'Y', sf::Color::Yellow},
    {'M', sf::Color::Magenta},
    {'C', sf::Color::Cyan},
    {'O', sf::Color(255, 165, 0)},
    {'P', sf::Color(128, 0, 128)}};

std::vector<ColorNodes>
    colorNodes;
std::vector<std::vector<bool>> cellExists;

FlowGrid readLevel(int level, sf::RenderWindow &window)
{
    colorNodes.clear();
    cellExists.clear();

    std::ifstream file(Defaults::HOME_DIR + "/src/levels/" + std::to_string(level) + ".txt");
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open level file." << std::endl;
        return FlowGrid(0, 0, 0, colorNodes, cellExists, window);
    }

    u_short rows, cols;
    file >> rows >> cols;

    cellExists.resize(rows, std::vector<bool>(cols, false));

    for (u_short row = 0; row < rows; ++row)
    {
        for (u_short col = 0; col < cols; ++col)
        {
            char cellType;
            file >> cellType;
            if (cellType == '#')
            {
                cellExists[row][col] = true;
            }
            else if (colorMap.find(cellType) != colorMap.end())
            {
                sf::Color color = colorMap[cellType];
                colorNodes.push_back({row, col, color});
                cellExists[row][col] = true;
            }
        }
        if (file.peek() == '\n')
        {
            file.ignore();
        }
    }

    return FlowGrid(rows, cols, Defaults::CELL_SIZE, colorNodes, cellExists, window);
}