#pragma once
// #include <iostream>
#include <SFML/Graphics.hpp>
#include <functional>

class Button
{
public:
    Button()
    {
        font = sf::Font("src/ARIAL.TTF");
    };
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
    void setText(const std::string &label)
    {
        text.setString(label);
        text.setPosition(sf::Vector2f(
            shape.getPosition().x + (shape.getSize().x - text.getLocalBounds().size.x) / 2,
            shape.getPosition().y + (shape.getSize().y - text.getLocalBounds().size.y) / 2 - 5));
    }
    void setSize(sf::Vector2f size)
    {
        shape.setSize(size);
    }
    void setPosition(sf::Vector2f position)
    {
        shape.setPosition(position);
        text.setPosition(sf::Vector2f(
            position.x + (shape.getSize().x - text.getLocalBounds().size.x) / 2,
            position.y + (shape.getSize().y - text.getLocalBounds().size.y) / 2 - 5));
    }
    void setColor(sf::Color color)
    {
        shape.setFillColor(color);
    }
    void setOutlineColor(sf::Color color)
    {
        shape.setOutlineColor(color);
    }
    void setOutlineThickness(float thickness)
    {
        shape.setOutlineThickness(thickness);
    }
    void setTextColor(sf::Color color)
    {
        text.setFillColor(color);
    }

    sf::Vector2f getSize()
    {
        return shape.getSize();
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