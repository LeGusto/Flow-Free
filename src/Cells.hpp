#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#pragma once

struct Cell {
    u_short row = 0;
    u_short col = 0;
    sf::RectangleShape shape = sf::RectangleShape();
    sf::Color color = sf::Color::Black;
    
    void virtual draw_cell(sf::RenderWindow& window) { window.draw(shape); }
    sf::Color virtual getColor() const { return color; };
    void setColor(sf::Color color) { this->color = color; shape.setFillColor(color); }


    bool colorNode = false;

    Cell(u_short row, u_short col, sf::Vector2f position, 
        u_short cellSize, u_short gridLineThickness)
        : row(row), col(col) {
            shape.setPosition(position);
            shape.setFillColor(color);
            shape.setSize(sf::Vector2f(cellSize, cellSize));
            shape.setOutlineThickness(gridLineThickness);
            shape.setOutlineColor(sf::Color(128, 128, 128));
        }
    
    
};

struct SourceCell : public Cell {

    sf::CircleShape* circle = nullptr;

    SourceCell(u_short row, u_short col, sf::Vector2f position, 
        u_short cellSize, u_short gridLineThickness, sf::CircleShape& circle)
        : Cell(row, col, position, cellSize, gridLineThickness) {
            this->colorNode = true;
            this->circle = &circle;
        }
    
    void draw_cell(sf::RenderWindow& window) override {
        window.draw(shape);
        if (circle) {
            window.draw(*circle);
        }
    }

    sf::Color getColor() const override {
        return circle->getFillColor();
    }
};

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