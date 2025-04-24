#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Defaults.hpp"

#pragma once

struct Cell
{
    u_short row = 0;
    u_short col = 0;
    bool colorNode = false;
    sf::RectangleShape shape = sf::RectangleShape();
    sf::Color color = Defaults::CELL_COLOR;
    int path = -1; // Which path it belongs to

    void virtual draw_cell(sf::RenderWindow &window)
    {
        window.draw(shape);
    }
    sf::Color virtual getColor() const
    {
        return color;
    };
    void virtual setColor(sf::Color color)
    {
        this->color = color;
        // shape.setFillColor(color);
    }
    void virtual setOutlineColor(sf::Color color)
    {
        shape.setOutlineColor(color);
    }
    void virtual setOrigin(sf::Vector2f origin)
    {
        shape.setOrigin(origin);
    }
    void addToPath()
    {
        this->path = path;
        setColor(color);
        setOutlineColor(color);
    }

    Cell(u_short row, u_short col, sf::Vector2f position,
         u_short cellSize, u_short gridLineThickness)
        : row(row), col(col)
    {
        shape.setPosition(position);
        shape.setFillColor(Defaults::CELL_COLOR);
        shape.setSize(sf::Vector2f(cellSize, cellSize));
        shape.setOutlineThickness(gridLineThickness);
        shape.setOutlineColor(Defaults::OUTLINE_COLOR);
    }
};

struct SourceCell : public Cell
{

    sf::CircleShape *circle = nullptr;

    SourceCell(u_short row, u_short col, sf::Vector2f position,
               u_short cellSize, u_short gridLineThickness, sf::CircleShape &circle)
        : Cell(row, col, position, cellSize, gridLineThickness)
    {
        this->colorNode = true;
        this->circle = &circle;
    }

    void draw_cell(sf::RenderWindow &window) override
    {
        window.draw(shape);
        if (circle)
        {
            window.draw(*circle);
        }
    }
    void setColor(sf::Color color) override
    {
        this->color = color;
    }
    sf::Color getColor() const override
    {
        return circle->getFillColor();
    }
    void setOrigin(sf::Vector2f origin) override
    {
        shape.setOrigin(origin);
        if (circle)
        {
            circle->setOrigin(origin);
        }
    }
};

struct BlockingCell : public Cell
{
    BlockingCell(u_short row, u_short col, sf::Vector2f position,
                 u_short cellSize, u_short gridLineThickness)
        : Cell(row, col, position, cellSize, gridLineThickness)
    {
        this->colorNode = true;
        this->color = Defaults::OUTLINE_COLOR;
        this->shape.setFillColor(Defaults::OUTLINE_COLOR);
    }

    void setColor(sf::Color color) override { return; }
};

// Store source noces
struct ColorNodes
{
public:
    ColorNodes(u_short row, u_short col, sf::Color color)
        : row(row), col(col), color(color) {}
    const u_short row = 0;
    const u_short col = 0;
    sf::CircleShape circle;

    const sf::Color color = Defaults::COLORNODE_COLOR;
};
