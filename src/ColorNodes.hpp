#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#pragma once

// Store pairs of color nodes in the grid
struct ColorNodes {
public: 
    ColorNodes(u_short row1, u_short col1, u_short row2, u_short col2, sf::Color color) 
    : row1(row1), col1(col1), row2(row2), col2(col2), color(color) {}

    const u_short row1 = 0;
    const u_short col1 = 0;
    sf::CircleShape circle1;

    const u_short row2 = 0;
    const u_short col2 = 0;
    sf::CircleShape circle2;

    const sf::Color color = sf::Color::White;
};