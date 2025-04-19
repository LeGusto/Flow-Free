#include "Grid.hpp"
#include <iostream>

// Store pairs of color nodes in the grid
struct ColorNodes {
    u_short row1 = 0;
    u_short col1 = 0;
    sf::CircleShape circle1;

    u_short row2 = 0;
    u_short col2 = 0;
    sf::CircleShape circle2;

    sf::Color color = sf::Color::White;

    ColorNodes(u_short row1, u_short col1, u_short row2, u_short col2, sf::Color color) 
        : row1(row1), col1(col1), row2(row2), col2(col2), color(color) {}
};

class FlowGrid : public Grid {
public:
    FlowGrid(u_short rows, u_short cols, u_short cellSize, std::vector<ColorNodes> colorNodes) {
        this->cellSize = cellSize;
        this->rows = rows;
        this->cols = cols;
        this->colorNodes = colorNodes;

        cells.resize(rows, std::vector<sf::RectangleShape>(cols));
        initializeShapes(rows, cols, cellSize, colorNodes);
        updateGrid();
    };


    u_short getCellSize() const override { return cellSize; }
    u_short getGridLineThickness() const override { return gridLineThickness; }
    u_short getRows() const override { return rows; }
    u_short getCols() const override { return cols; }
    u_int getGridSize() const override { return (rows + 1) * gridLineThickness 
                                            + (cols + 1) * gridLineThickness;
                                            + cellSize * rows * cols; }

    // Applies any modifications to the grid
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

        for (auto& node : colorNodes) {
            node.circle1.setFillColor(node.color);
            node.circle1.setPosition(get_pos(node.row1, node.col1));
            node.circle1.setRadius(cellSize / 2);

            node.circle2.setFillColor(node.color);
            node.circle2.setPosition(get_pos(node.row2, node.col2));
            node.circle2.setRadius(cellSize / 2);
        }
    }

    void draw(sf::RenderWindow& window) {
        for (u_short row = 0; row < rows; ++row) {
            for (u_short col = 0; col < cols; ++col) {
                window.draw(cells[row][col]);
            }
        }

        for (auto& node : colorNodes) {
            window.draw(node.circle1);
            window.draw(node.circle2);
        }
    }

    void initializeShapes(u_short rows, u_short cols, u_short cellSize, std::vector<ColorNodes> colorNodes) {
        for (u_short row = 0; row < rows; ++row) {
            for (u_short col = 0; col < cols; ++col) {
                cells[row][col] = sf::RectangleShape();
            }
        }

        for (auto& node : colorNodes) {
            node.circle1 = sf::CircleShape();
            node.circle2 = sf::CircleShape();
        }
    }

    void setCellSize(u_short cellSize) override {
        this->cellSize = cellSize;
        updateGrid();
    };
    void setCellColor(u_short row, u_short col, const sf::Color& color) override{};

private:
    u_short cellSize = 0;
    u_short gridLineThickness = 5;
    u_short rows = 0;
    u_short cols = 0;
    std::vector<std::vector<sf::RectangleShape>> cells = {};
    std::vector<ColorNodes> colorNodes = {};
};