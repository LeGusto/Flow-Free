#pragma once
// #include <iostream>
#include <SFML/Graphics.hpp>
#include <functional>

class Button
{
public:
    Button(float x, float y, float width, float height, sf::Color color, const std::string &label)
    {
        font = sf::Font("src/ARIAL.TTF");

        shape.setPosition(sf::Vector2f(x, y));
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(color);
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color::Black);

        text.setString(label);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);
        text.setPosition(sf::Vector2f(
            x + (width - text.getLocalBounds().size.x) / 2,
            y + (height - text.getLocalBounds().size.y) / 2 - 5));
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(shape);
        window.draw(text);
    }

    bool isClicked(const sf::Vector2i &mousePos)
    {
        return shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    }

private:
    mutable sf::Font font;
    sf::Text text = sf::Text(font);
    sf::RectangleShape shape;
};