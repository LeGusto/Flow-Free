#include "Grid.hpp"
#include "ColorNodes.hpp"
#include <iostream>

#pragma once

class FlowGrid : public Grid {
public:
    FlowGrid(u_short rows, u_short cols, u_short cellSize, std::vector<ColorNodes> colorNodes)
        : rows(rows), cols(cols), cellSize(cellSize), colorNodes(std::move(colorNodes)) {
        cells.resize(rows, std::vector<sf::RectangleShape>(cols));
        initializeShapes();
    }

    u_short getCellSize() const override { return cellSize; }
    u_short getGridLineThickness() const override { return gridLineThickness; }
    u_short getRows() const override { return rows; }
    u_short getCols() const override { return cols; }
    u_int getGridSize() const override { return (rows + 1) * gridLineThickness 
                                            + (cols + 1) * gridLineThickness
                                            + cellSize * rows * cols; }

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

    sf::Vector2f getCellPos(u_short row, u_short col) {
        return sf::Vector2f(
            col * cellSize + (col + 1) * gridLineThickness, 
            row * cellSize + (row + 1) * gridLineThickness
        );
    }

    void initializeShapes() {
        for (u_short row = 0; row < rows; ++row) {
            for (u_short col = 0; col < cols; ++col) {
                cells[row][col] = sf::RectangleShape();
                cells[row][col].setSize(sf::Vector2f(cellSize, cellSize));
                cells[row][col].setPosition(getCellPos(row, col));
                cells[row][col].setFillColor(sf::Color::White);
                cells[row][col].setOutlineThickness(gridLineThickness);
                cells[row][col].setOutlineColor(sf::Color::Green);
            }
        }

        for (auto& node : colorNodes) {
            if (node.row1 == node.row2 && node.col1 == node.col2) {
                std::cerr << "Error: ColorNodes cannot have the same position for both circles." << std::endl;
                continue;
            }
            if (node.row1 >= rows || node.col1 >= cols || node.row2 >= rows || node.col2 >= cols) {
                std::cerr << "Error: ColorNodes position out of bounds." << std::endl;
                continue;
            }

            node.circle1 = sf::CircleShape();
            node.circle1.setFillColor(node.color);
            node.circle1.setPosition(getCellPos(node.row1, node.col1));
            node.circle1.setRadius(cellSize / 2);

            node.circle2 = sf::CircleShape();
            node.circle2.setFillColor(node.color);
            node.circle2.setPosition(getCellPos(node.row2, node.col2));
            node.circle2.setRadius(cellSize / 2);
        }
    }

    void setCellSize(u_short cellSize) override {
        this->cellSize = cellSize;
        for (u_short row = 0; row < rows; ++row) {
            for (u_short col = 0; col < cols; ++col) {
                cells[row][col].setSize(sf::Vector2f(cellSize, cellSize));
                cells[row][col].setPosition(getCellPos(row, col));
            }
        }

        for (auto& node : colorNodes) {
            node.circle1.setPosition(getCellPos(node.row1, node.col1));
            node.circle1.setRadius(cellSize / 2);

            node.circle2.setPosition(getCellPos(node.row2, node.col2));
            node.circle2.setRadius(cellSize / 2);
        }
    };

private:
    u_short cellSize = 0;
    u_short gridLineThickness = 5;
    u_short rows = 0;
    u_short cols = 0;
    std::vector<std::vector<sf::RectangleShape>> cells = {};
    std::vector<ColorNodes> colorNodes = {};
};