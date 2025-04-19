#include "Grid.hpp"
#include <iostream>

class FlowGrid : public Grid {
public:
    FlowGrid(u_short rows, u_short cols, u_short cellSize)
        : Grid(rows, cols, cellSize) {
            this->cellSize = cellSize;
            this->rows = rows;
            this->cols = cols;

            cells.resize(rows, std::vector<sf::RectangleShape>(cols));
            for (u_short row = 0; row < rows; ++row) {
                for (u_short col = 0; col < cols; ++col) {
                    cells[row][col] = sf::RectangleShape();
                }
            }
            updateGrid();
        };


    u_short getCellSize() const override { return cellSize; }
    u_short getGridLineThickness() const override { return gridLineThickness; }
    u_short getRows() const override { return rows; }
    u_short getCols() const override { return cols; }

    void updateGrid() {
        auto get_pos = [&](u_short& row, u_short& col) {
            return sf::Vector2f(
                col * this->cellSize + (col + 1) * this->gridLineThickness, 
                row * this->cellSize + (row + 1) * this->gridLineThickness
            );
        };

        for (u_short row = 0; row < rows; ++row) {
            for (u_short col = 0; col < cols; ++col) {
                cells[row][col].setSize(sf::Vector2f(this->cellSize, this->cellSize));
                cells[row][col].setPosition(get_pos(row, col));
                cells[row][col].setFillColor(sf::Color::White);
                cells[row][col].setOutlineThickness(gridLineThickness);
                cells[row][col].setOutlineColor(sf::Color::Green);
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        for (u_short row = 0; row < rows; ++row) {
            for (u_short col = 0; col < cols; ++col) {
                window.draw(cells[row][col]);
            }
        }
    }

    void setCellSize(u_short cellSize) override {};
    void setCellColor(u_short row, u_short col, const sf::Color& color) override{};

private:
    u_short cellSize = 0;
    u_short gridLineThickness = 5;
    u_short rows = 0;
    u_short cols = 0;
    std::vector<std::vector<sf::RectangleShape>> cells = {};

};