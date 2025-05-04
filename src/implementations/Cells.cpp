#include "../headers/Cells.hpp"

Cell::Cell(u_short row, u_short col, sf::Vector2f position, u_short cellSize, u_short gridLineThickness)
    : row(row), col(col), colorNode(false), path(-1), shape(sf::RectangleShape()), color(Defaults::CELL_COLOR)
{
    shape.setPosition(position);
    shape.setFillColor(Defaults::CELL_COLOR);
    shape.setSize(sf::Vector2f(cellSize, cellSize));
    shape.setOutlineThickness(gridLineThickness);
    shape.setOutlineColor(Defaults::OUTLINE_COLOR);
}

void Cell::draw_cell(sf::RenderWindow &window)
{
    window.draw(shape);
}

sf::Color Cell::getColor() const
{
    return color;
}

void Cell::setColor(sf::Color color)
{
    this->color = color;
}

void Cell::setOutlineColor(sf::Color color)
{
    shape.setOutlineColor(color);
}

void Cell::setOrigin(sf::Vector2f origin)
{
    shape.setOrigin(origin);
}

void Cell::addToPath()
{
    this->path = path;
    setColor(color);
    setOutlineColor(color);
}

SourceCell::SourceCell(u_short row, u_short col, sf::Vector2f position, u_short cellSize, u_short gridLineThickness, sf::CircleShape &circle)
    : Cell(row, col, position, cellSize, gridLineThickness), circle(&circle)
{
    this->colorNode = true;
}

void SourceCell::draw_cell(sf::RenderWindow &window)
{
    window.draw(shape);
    if (circle)
    {
        window.draw(*circle);
    }
}

void SourceCell::setColor(sf::Color color)
{
    this->color = color;
}

sf::Color SourceCell::getColor() const
{
    return circle->getFillColor();
}

void SourceCell::setOrigin(sf::Vector2f origin)
{
    shape.setOrigin(origin);
    if (circle)
    {
        circle->setOrigin(origin);
    }
}

BlockingCell::BlockingCell(u_short row, u_short col, sf::Vector2f position, u_short cellSize, u_short gridLineThickness)
    : Cell(row, col, position, cellSize, gridLineThickness)
{
    this->colorNode = true;
    this->color = Defaults::OUTLINE_COLOR;
    this->shape.setFillColor(Defaults::OUTLINE_COLOR);
}

void BlockingCell::setColor(sf::Color color)
{
    return;
}

ColorNodes::ColorNodes(u_short row, u_short col, sf::Color color)
    : row(row), col(col), color(color)
{
}