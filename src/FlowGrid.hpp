#pragma once

#include "Grid.hpp"
#include "Cells.hpp"
#include "PathManager.hpp"
#include "Defaults.hpp"
#include "ActionManager.hpp"
#include <iostream>

class FlowGrid : public Grid
{
public:
    FlowGrid() = delete;
    FlowGrid(u_short rows, u_short cols, u_short cellSize, std::vector<ColorNodes> colorNodes, std::vector<std::vector<bool>> cellExists, sf::RenderWindow &window)
        : rows(rows), cols(cols), cellSize(cellSize), colorNodes(std::move(colorNodes))
    {
        origin = sf::Vector2f(
            -(float)(window.getSize().x - (cols * cellSize + (cols + 1) * gridLineThickness)) / 2,
            -(float)(window.getSize().y - (rows * cellSize + (rows + 1) * gridLineThickness)) / 2);
        remainingSources = this->colorNodes.size() / 2;
        cells.resize(rows, std::vector<Cell *>(cols, nullptr));
        initializeShapes(cellExists);
        initializeButtons();
    }

    // FlowGrid &operator=(const FlowGrid &other) = delete;
    FlowGrid &operator=(FlowGrid &&other) noexcept
    {
        if (this != &other)
        {
            cellSize = other.cellSize;
            gridLineThickness = other.gridLineThickness;
            rows = other.rows;
            cols = other.cols;
            remainingSources = other.remainingSources;

            origin = std::move(other.origin);

            pathTemp = std::move(other.pathTemp);
            cells = std::move(other.cells);
            colorNodes = std::move(other.colorNodes);

            pathMaker = std::move(other.pathMaker);
            actionManager = std::move(other.actionManager);
            undoButton = std::move(other.undoButton);
            redoButton = std::move(other.redoButton);
            returnButton = std::move(other.returnButton);

            pathMaker.updateRemainingSources(&remainingSources);

            // Prevent destructor from accesing unstable entries
            other.rows = 0;
            other.cols = 0;
        }
        return *this;
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
        cells.clear();
    }

    u_short getCellSize() const override { return cellSize; }
    u_short getGridLineThickness() const override { return gridLineThickness; }
    u_short getRows() const override { return rows; }
    u_short getCols() const override { return cols; }
    sf::Vector2f getGridSize() const override
    {
        return sf::Vector2f(
            cols * cellSize + (cols + 1) * gridLineThickness,
            rows * cellSize + (rows + 1) * gridLineThickness);
    }
    bool isDrawing() { return pathMaker.isPathDrawing(); }

    void draw(sf::RenderWindow &window)
    {
        Cell *pathMakerHead = pathMaker.getLastCell();
        for (u_short row = 0; row < rows; ++row)
        {
            for (u_short col = 0; col < cols; ++col)
            {
                if (cells[row][col] == nullptr)
                    continue;
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
        if (remainingSources != 0)
        {
            undoButton.draw(window);
            redoButton.draw(window);
            returnButton.draw(window);
        }
    }

    sf::Vector2f getCellPos(u_short row, u_short col)
    {
        return sf::Vector2f(
            col * cellSize + (col + 1) * gridLineThickness,
            row * cellSize + (row + 1) * gridLineThickness);
    }

    void initializeShapes(std::vector<std::vector<bool>> &cellExists)
    {

        for (auto &node : colorNodes)
        {

            if (node.row >= rows || node.col >= cols)
            {
                std::cerr << "Error: ColorNodes position out of bounds." << std::endl;
                continue;
            }

            node.circle = sf::CircleShape();
            node.circle.setFillColor(node.color);
            node.circle.setPosition(getCellPos(node.row, node.col));
            node.circle.setRadius(cellSize / 2);
            cells[node.row][node.col] = new SourceCell(node.row, node.col, getCellPos(node.row, node.col), cellSize, gridLineThickness, node.circle);
        }

        for (u_short row = 0; row < rows; ++row)
        {
            for (u_short col = 0; col < cols; ++col)
            {
                if (cells[row][col] == nullptr)
                {
                    if (cellExists[row][col])
                    {
                        cells[row][col] = new Cell(row, col, getCellPos(row, col), cellSize, gridLineThickness);
                    }
                    else
                    {
                        cells[row][col] = new BlockingCell(row, col, getCellPos(row, col), cellSize, gridLineThickness);
                    }
                }
                cells[row][col]->setOrigin(origin);
            }
        }
    }

    void initializeButtons()
    {
        float PADDING = Defaults::BUTTON_PADDING;
        float startX = -origin.x + getGridSize().x / 2 - (3 * (100 + PADDING)) / (float)2;

        undoButton.setPosition(sf::Vector2f(startX, -origin.y - 60));
        redoButton.setPosition(sf::Vector2f(startX + redoButton.getSize().x + PADDING, -origin.y - 60));
        returnButton.setPosition(sf::Vector2f(startX + 2 * (returnButton.getSize().x + PADDING), -origin.y - 60));
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
            node.circle.setRadius(cellSize / 2);
            node.circle.setPosition(getCellPos(node.row, node.col));
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

            if (pathMaker.addCells(pathTemp) && !pathMaker.isPathDrawing())
            {
                actionManager.undoAddAction(Action(Action::ADD, pathMaker.getPath(prevCell->getColor().toInteger())));
            }
        }
        else if (cell->colorNode)
        {
            pathMaker.startPath(cell, cell->getColor());
        }
    }

    // Get the path from the current cell to the previous cell
    void tracePath(Cell *cell, Cell *prevCell)
    {
        if (cell->row == prevCell->row && cell->col == prevCell->col)
        {
            pathTemp.push_back(cell);
            return;
        }
        if (cell->row == prevCell->row)
        {
            int step = (cell->col - prevCell->col) / abs(cell->col - prevCell->col);
            for (int c = prevCell->col + step; c != cell->col; c += step)
            {
                pathTemp.push_back(cells[cell->row][c]);
            }
        }
        else if (cell->col == prevCell->col)
        {
            int step = (cell->row - prevCell->row) / abs(cell->row - prevCell->row);
            for (int r = prevCell->row + step; r != cell->row; r += step)
            {
                pathTemp.push_back(cells[r][cell->col]);
            }
        }
        pathTemp.push_back(cell);
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

    sf::Vector2f getOrigin()
    {
        return origin;
    }

    void undo()
    {
        actionManager.undo();
    }

    void redo()
    {
        actionManager.redo();
    }

    bool isCompleted()
    {
        return remainingSources == 0;
    }

    bool processEvent(const std::optional<sf::Event> &event, sf::RenderWindow &window, std::string &response)
    {
        if (const auto *mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseButtonPressed->button == sf::Mouse::Button::Left)
            {
                if (undoButton.isClicked(mouseButtonPressed->position))
                {
                    response = "Undo";
                    undo();
                }
                else if (redoButton.isClicked(mouseButtonPressed->position))
                {
                    response = "Redo";
                    redo();
                }
                else if (returnButton.isClicked(mouseButtonPressed->position))
                {
                    response = "Return";
                }
                else
                {
                    makePath(mouseButtonPressed->position.x, mouseButtonPressed->position.y);
                }
                return true;
            }
            else if (mouseButtonPressed->button == sf::Mouse::Button::Right)
            {
                if (isDrawing())
                {
                    destroyPath();
                }
                return true;
            }
        }

        if (const auto *mouseMoved = event->getIf<sf::Event::MouseMoved>())
        {
            if (isDrawing())
            {
                makePath(mouseMoved->position.x, mouseMoved->position.y);
            }
        }
        return false;
    }

private:
    u_short cellSize = 0;
    u_short gridLineThickness = Defaults::GRID_LINE_THICKNESS;
    u_short rows = 0;
    u_short cols = 0;
    u_short remainingSources = 0;
    sf::Vector2f origin = {-50, -50};
    std::vector<Cell *> pathTemp = {};

    std::vector<std::vector<Cell *>> cells = {};
    std::vector<ColorNodes> colorNodes = {};
    PathMaker pathMaker = PathMaker(&origin, &remainingSources);
    ActionManager actionManager = ActionManager();
    Button undoButton = makeUndoButton();
    Button redoButton = makeRedoButton();
    Button returnButton = makeReturnButton();
};