#pragma once

#include <iostream>
#include "Button.hpp"
#include <SFML/Graphics.hpp>

class CompletedScreen
{
public:
    CompletedScreen(sf::RenderWindow &window);
    ~CompletedScreen();

    void handleClick(const std::optional<sf::Event> &event, std::string &response);
    void draw(sf::RenderWindow &window);

private:
    sf::RectangleShape menu;
    sf::Font font;
    sf::Text title = sf::Text(font);
};