#pragma once

#include <filesystem>
#include <SFML/Graphics.hpp>

namespace Defaults
{
    const std::string HOME_DIR = std::filesystem::current_path().string();

    constexpr u_short CELL_SIZE = 50;
    constexpr u_short GRID_LINE_THICKNESS = 5;
    constexpr u_short ROWS = 10;
    constexpr u_short COLS = 10;
    constexpr float PATH_THICKNESS = 0.3f;
    constexpr float PATH_SENSITIVITY = 0.4f;

    const sf::Color CELL_COLOR = sf::Color::Black;
    const sf::Color OUTLINE_COLOR = sf::Color(128, 128, 128);
    const sf::Color COLORNODE_COLOR = sf::Color::Black;

    const sf::Vector2f MENU_BUTTON_SIZE = sf::Vector2f(200, 50);
    const sf::Vector2f ACTION_BUTTON_SIZE = sf::Vector2f(100, 50);
    const int BUTTON_OUTLINE_THICKNESS = 2;
    const int BUTTON_PADDING = 20;
}