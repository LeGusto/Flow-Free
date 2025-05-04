#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <functional>
#include "Defaults.hpp"
#include <filesystem>

class Button
{
public:
    Button();
    Button(const Button &other);
    ~Button();

    void setText(const std::string &label);
    void setSize(sf::Vector2f size);
    void setPosition(sf::Vector2f position);
    void setColor(sf::Color color);
    void setOutlineColor(sf::Color color);
    void setOutlineThickness(float thickness);
    void setTextColor(sf::Color color);
    void setTextSize(u_short size);
    sf::Vector2f getSize();
    void setTextRotation(sf::Angle angle);
    void draw(sf::RenderWindow &window);
    bool isClicked(const sf::Vector2i &mousePos);

private:
    sf::Font font;
    sf::Text text = sf::Text(font);
    sf::RectangleShape shape;
};

Button makeReturnButton();
Button makeLevelButton();
Button makeUndoButton();
Button makeRedoButton();
Button makeStartButton();
Button makeExitButton();