#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Defaults.hpp"

struct Cell
{
    unsigned short row;
    unsigned short col;
    bool colorNode;
    int path;
    sf::RectangleShape shape;
    sf::Color color;

    Cell(unsigned short row, unsigned short col, sf::Vector2f position, unsigned short cellSize, unsigned short gridLineThickness);

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

    SourceCell(unsigned short row, unsigned short col, sf::Vector2f position, unsigned short cellSize, unsigned short gridLineThickness, sf::CircleShape &circle);

    void draw_cell(sf::RenderWindow &window) override;
    void setColor(sf::Color color) override;
    sf::Color getColor() const override;
    void setOrigin(sf::Vector2f origin) override;
};

struct BlockingCell : public Cell
{
    BlockingCell(unsigned short row, unsigned short col, sf::Vector2f position, unsigned short cellSize, unsigned short gridLineThickness);

    void setColor(sf::Color color) override;
};

struct ColorNodes
{
    unsigned short row;
    unsigned short col;
    sf::CircleShape circle;
    sf::Color color;

    ColorNodes(unsigned short row, unsigned short col, sf::Color color);
};
