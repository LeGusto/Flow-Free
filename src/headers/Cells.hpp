#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Defaults.hpp"

struct Cell
{
    u_short row;
    u_short col;
    bool colorNode;
    int path;
    sf::RectangleShape shape;
    sf::Color color;

    Cell(u_short row, u_short col, sf::Vector2f position, u_short cellSize, u_short gridLineThickness);

    virtual void draw_cell(sf::RenderWindow &window);
    virtual sf::Color getColor() const;
    virtual void setColor(sf::Color color);
    virtual void setOutlineColor(sf::Color color);
    virtual void setOrigin(sf::Vector2f origin);
    void addToPath();
};

struct SourceCell : public Cell
{
    sf::CircleShape *circle = nullptr;

    SourceCell(u_short row, u_short col, sf::Vector2f position, u_short cellSize, u_short gridLineThickness, sf::CircleShape &circle);

    void draw_cell(sf::RenderWindow &window) override;
    void setColor(sf::Color color) override;
    sf::Color getColor() const override;
    void setOrigin(sf::Vector2f origin) override;
};

struct BlockingCell : public Cell
{
    BlockingCell(u_short row, u_short col, sf::Vector2f position, u_short cellSize, u_short gridLineThickness);

    void setColor(sf::Color color) override;
};

struct ColorNodes
{
    u_short row;
    u_short col;
    sf::CircleShape circle;
    sf::Color color;

    ColorNodes(u_short row, u_short col, sf::Color color);
};
