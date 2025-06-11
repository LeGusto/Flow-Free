#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

class Grid
{
public:
    virtual void setCellSize(unsigned short cellSize) = 0;

    virtual unsigned short getCellSize() const = 0;
    virtual unsigned short getGridLineThickness() const = 0;
    virtual unsigned short getRows() const = 0;
    virtual unsigned short getCols() const = 0;
    virtual sf::Vector2f getGridSize() const = 0;
};