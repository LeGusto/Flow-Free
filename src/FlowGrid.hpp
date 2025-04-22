#pragma once

#include "Grid.hpp"
#include "Cells.hpp"
#include "Path.hpp"
#include "Defaults.hpp"
#include <iostream>

class FlowGrid : public Grid
{
public:
    FlowGrid(u_short rows, u_short cols, u_short cellSize, std::vector<ColorNodes> colorNodes, sf::RenderWindow &window)
        : rows(rows), cols(cols), cellSize(cellSize), colorNodes(std::move(colorNodes))
    {
        origin = sf::Vector2f(
            -(float)(window.getSize().x - (cols * cellSize + (cols + 1) * gridLineThickness)) / 2,
            -(float)(window.getSize().y - (rows * cellSize + (rows + 1) * gridLineThickness)) / 2);

        cells.resize(rows, std::vector<Cell *>(cols, nullptr));
        initializeShapes();
    }

    u_short getCellSize() const override { return cellSize; }
    u_short getGridLineThickness() const override { return gridLineThickness; }
    u_short getRows() const override { return rows; }
    u_short getCols() const override { return cols; }
    u_int getGridSize() const override { return (rows + 1) * gridLineThickness + (cols + 1) * gridLineThickness + cellSize * rows * cols; }
    bool isDrawing() { return pathMaker.isPathDrawing(); }

    void draw(sf::RenderWindow &window)
    {
        Cell *pathMakerHead = pathMaker.getLastCell();
        for (u_short row = 0; row < rows; ++row)
        {
            for (u_short col = 0; col < cols; ++col)
            {
                if (cells[row][col] == pathMakerHead)
                    continue;
                cells[row][col]->draw_cell(window);
            }
        }
        if (pathMakerHead != nullptr)
        {
            pathMakerHead->draw_cell(window);
        }
        pathMaker.drawPaths(window);
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
                if (cells[row][col] == nullptr)
                {
                    cells[row][col] = new Cell(row, col, getCellPos(row, col), cellSize, gridLineThickness);
                }
                cells[row][col]->setOrigin(origin);
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
        x += origin.x;
        y += origin.y;
        // Out of bounds
        if (y / (cellSize + gridLineThickness) >= rows || x / (cellSize + gridLineThickness) >= cols || x < 0 || y < 0)
        {
            return;
        }

        Cell *cell = cells[y / (cellSize + gridLineThickness)][x / (cellSize + gridLineThickness)];
        if (!cell)
        {
            std::cerr << "Error: Missing cell at position (" << x << ", " << y << ")." << std::endl;
            return;
        }

        if (pathMaker.isPathDrawing())
        {
            // Adjust sensitivity of drawing
            u_short x_pad = abs((cellSize + gridLineThickness) / 2 - x % (cellSize + gridLineThickness));
            u_short y_pad = abs((cellSize + gridLineThickness) / 2 - y % (cellSize + gridLineThickness));
            if (x_pad > (cellSize + gridLineThickness) * Defaults::PATH_SENSITIVITY || y_pad > (cellSize + gridLineThickness) * Defaults::PATH_SENSITIVITY)
            {
                return;
            }

            pathTemp.clear();
            Cell *prevCell = pathMaker.getLastCell();
            if (prevCell == nullptr)
            {
                std::cerr << "Error: No previous cell found." << std::endl;
                return;
            }

            tracePath(cell, prevCell);

            if (pathTemp.empty())
            {
                pathTemp.push_back(cell);
            }

            pathMaker.addCells(pathTemp);
        }
        else if (cell->colorNode)
        {
            pathMaker.startPath(cell, cell->getColor());
        }
    }

    void tracePath(Cell *cell, Cell *prevCell)
    {
        if (cell->row == prevCell->row)
        {
            for (int c = std::min(cell->col, prevCell->col); c <= std::max(cell->col, prevCell->col); ++c)
            {
                pathTemp.push_back(cells[cell->row][c]);
            }
            if (cell->col - prevCell->col < 0)
                reverse(pathTemp.begin(), pathTemp.end());
        }
        else if (cell->col == prevCell->col)
        {
            for (int r = std::min(cell->row, prevCell->row); r <= std::max(cell->row, prevCell->row); ++r)
            {
                pathTemp.push_back(cells[r][cell->col]);
            }
            if (cell->row - prevCell->row < 0)
                reverse(pathTemp.begin(), pathTemp.end());
        }
    }

    void destroyPath()
    {
        pathMaker.destroyPath();
    }

    Cell *getCell(int row, int col)
    {
        if (row >= rows || col >= cols)
        {
            std::cerr << "Error: Cell out of bounds." << std::endl;
            return nullptr;
        }
        return cells[row][col];
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
    u_short gridLineThickness = Defaults::GRID_LINE_THICKNESS;
    u_short rows = 0;
    u_short cols = 0;
    sf::Vector2f origin = {-50, -50};
    std::vector<Cell *> pathTemp = {};

    std::vector<std::vector<Cell *>> cells = {};
    std::vector<ColorNodes> colorNodes = {};
    PathMaker pathMaker = PathMaker(&origin);
};