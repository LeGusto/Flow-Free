#include "../headers/Cells.hpp"

// Initializes a rectangular cell with the given position, size, and grid line thickness
Cell::Cell(unsigned short row, unsigned short col, sf::Vector2f position, unsigned short cellSize, unsigned short gridLineThickness)
    : row(row), col(col), colorNode(false), path(-1), shape(sf::RectangleShape()), color(Defaults::CELL_COLOR)
{
    shape.setPosition(position);
    shape.setFillColor(Defaults::CELL_COLOR);
    shape.setSize(sf::Vector2f(cellSize, cellSize));
    shape.setOutlineThickness(gridLineThickness);
    shape.setOutlineColor(Defaults::OUTLINE_COLOR);
}

// Draws the cell on the SFML render window
void Cell::draw_cell(sf::RenderWindow &window)
{
    window.draw(shape);
}

// Returns the current color of the cell
sf::Color Cell::getColor() const
{
    return color;
}

// Sets the fill color of the cell
void Cell::setColor(sf::Color color)
{
    this->color = color;
}

// Sets the outline color of the cell
void Cell::setOutlineColor(sf::Color color)
{
    shape.setOutlineColor(color);
}

// Sets the origin of the cell for transformations
void Cell::setOrigin(sf::Vector2f origin)
{
    shape.setOrigin(origin);
}

// Marks the cell as part of a path and updates its color and outline
void Cell::addToPath()
{
    this->path = path; // Assign the path ID
    setColor(color);   // Update the fill color
    setOutlineColor(color);
}

// Initializes a source cell with a circular shape and a specific color
SourceCell::SourceCell(unsigned short row, unsigned short col, sf::Vector2f position, unsigned short cellSize, unsigned short gridLineThickness, sf::CircleShape &circle)
    : Cell(row, col, position, cellSize, gridLineThickness), circle(&circle)
{
    this->colorNode = true; // Mark the cell as a color node
}

// Draws the source cell on the SFML render window
void SourceCell::draw_cell(sf::RenderWindow &window)
{
    window.draw(shape);
    if (circle)
    {
        window.draw(*circle);
    }
}

// Sets the color of the source cell
void SourceCell::setColor(sf::Color color)
{
    this->color = color;
}

// Returns the color of the source cell
sf::Color SourceCell::getColor() const
{
    return circle->getFillColor();
}

// Sets the origin of the source cell for transformations
void SourceCell::setOrigin(sf::Vector2f origin)
{
    shape.setOrigin(origin);
    if (circle)
    {
        circle->setOrigin(origin);
    }
}

// Initializes a blocking cell with a specific color and appearance
BlockingCell::BlockingCell(unsigned short row, unsigned short col, sf::Vector2f position, unsigned short cellSize, unsigned short gridLineThickness)
    : Cell(row, col, position, cellSize, gridLineThickness)
{
    this->colorNode = true;                            // Mark the cell as a color node
    this->color = Defaults::OUTLINE_COLOR;             // Set the color to the default outline color
    this->shape.setFillColor(Defaults::OUTLINE_COLOR); // Set the fill color to the default outline color
}

// Overrides the setColor method for BlockingCell
// Blocking cells do not change color, so this method does nothing
void BlockingCell::setColor(sf::Color color)
{
    return;
}

// Initializes a color node with a specific row, column, and color
ColorNodes::ColorNodes(unsigned short row, unsigned short col, sf::Color color)
    : row(row), col(col), color(color)
{
}