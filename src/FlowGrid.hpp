#pragma once
#include "Grid.hpp"
#include "Cells.hpp"
#include "Path.hpp"
#include <iostream>

class FlowGrid : public Grid
{
public:
    FlowGrid(u_short rows, u_short cols, u_short cellSize, std::vector<ColorNodes> colorNodes)
        : rows(rows), cols(cols), cellSize(cellSize), colorNodes(std::move(colorNodes))
    {
        cells.resize(rows, std::vector<Cell *>(cols, nullptr));
        initializeShapes();
    }

    u_short getCellSize() const override { return cellSize; }
    u_short getGridLineThickness() const override { return gridLineThickness; }
    u_short getRows() const override { return rows; }
    u_short getCols() const override { return cols; }
    u_int getGridSize() const override { return (rows + 1) * gridLineThickness + (cols + 1) * gridLineThickness + cellSize * rows * cols; }
    bool isDrawing() { return path.isPathDrawing(); }

    void draw(sf::RenderWindow &window)
    {
        for (u_short row = 0; row < rows; ++row)
        {
            for (u_short col = 0; col < cols; ++col)
            {
                cells[row][col]->draw_cell(window);
            }
        }
    }

    sf::Vector2f getCellPos(u_short row, u_short col)
    {
        return sf::Vector2f(
            col * cellSize + (col + 1) * gridLineThickness,
            row * cellSize + (row + 1) * gridLineThickness);
    }

    void initializeShapes()
    {

        for (auto &node : colorNodes)
        {
            if (node.row1 == node.row2 && node.col1 == node.col2)
            {
                std::cerr << "Error: ColorNodes cannot have the same position for both circles." << std::endl;
                continue;
            }
            if (node.row1 >= rows || node.col1 >= cols || node.row2 >= rows || node.col2 >= cols)
            {
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

            cells[node.row1][node.col1] = new SourceCell(node.row1, node.col1, getCellPos(node.row1, node.col1), cellSize, gridLineThickness, node.circle1);
            cells[node.row2][node.col2] = new SourceCell(node.row2, node.col2, getCellPos(node.row2, node.col2), cellSize, gridLineThickness, node.circle2);
        }

        for (u_short row = 0; row < rows; ++row)
        {
            for (u_short col = 0; col < cols; ++col)
            {
                if (cells[row][col] != nullptr)
                    continue;
                cells[row][col] = new Cell(row, col, getCellPos(row, col), cellSize, gridLineThickness);
            }
        }
    }

    void setCellSize(u_short cellSize) override
    {
        this->cellSize = cellSize;
        for (u_short row = 0; row < rows; ++row)
        {
            for (u_short col = 0; col < cols; ++col)
            {
                cells[row][col]->shape.setSize(sf::Vector2f(cellSize, cellSize));
                cells[row][col]->shape.setPosition(getCellPos(row, col));
            }
        }

        for (auto &node : colorNodes)
        {
            node.circle1.setRadius(cellSize / 2);
            node.circle1.setPosition(getCellPos(node.row1, node.col1));

            node.circle2.setRadius(cellSize / 2);
            node.circle2.setPosition(getCellPos(node.row2, node.col2));
        }
    };

    void makePath(int x, int y)
    {
        // Out of bounds
        if (y / (cellSize + gridLineThickness) >= rows || x / (cellSize + gridLineThickness) >= cols)
        {
            return;
        }

        Cell *cell = cells[y / (cellSize + gridLineThickness)][x / (cellSize + gridLineThickness)];

        if (path.isPathDrawing())
        {
            path.addCell(cell);
        }
        else if (cell->colorNode)
        {
            path.startPath(cell, cell->getColor());
        }
    }

    void destroyPath()
    {
        path.clearPath();
    }

    ~FlowGrid()
    {
        for (u_short row = 0; row < rows; ++row)
        {
            for (u_short col = 0; col < cols; ++col)
            {
                delete cells[row][col];
            }
        }
    }

private:
    u_short cellSize = 0;
    u_short gridLineThickness = 5;
    u_short rows = 0;
    u_short cols = 0;
    std::vector<std::vector<Cell *>> cells = {};
    std::vector<ColorNodes> colorNodes = {};
    Path path;
};