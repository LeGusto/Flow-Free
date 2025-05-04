#pragma once

#include "Button.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>

class MainMenu
{
public:
    MainMenu(sf::RenderWindow &window);
    ~MainMenu();

    void handleClick(const std::optional<sf::Event> &event, std::string &response);
    void draw(sf::RenderWindow &window);

private:
    sf::RectangleShape menu;
    sf::Font font;
    sf::Text title = sf::Text(font);
    Button startButton = makeStartButton();
    Button exitButton = makeExitButton();
};