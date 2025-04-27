#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#pragma once

class Grid
{
public:
    virtual void setCellSize(u_short cellSize) = 0;

    virtual u_short getCellSize() const = 0;
    virtual u_short getGridLineThickness() const = 0;
    virtual u_short getRows() const = 0;
    virtual u_short getCols() const = 0;
    virtual sf::Vector2f getGridSize() const = 0;
};