#pragma once

#include "FlowGrid.hpp"
#include <unordered_map>
#include <vector>
#include <SFML/Graphics.hpp>

extern std::unordered_map<char, sf::Color> colorMap;
extern std::vector<ColorNodes> colorNodes;
extern std::vector<std::vector<bool>> cellExists;

FlowGrid readLevel(int level, sf::RenderWindow &window);