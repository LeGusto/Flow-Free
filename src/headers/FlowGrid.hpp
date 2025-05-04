#pragma once

#include "Grid.hpp"
#include "Cells.hpp"
#include "PathManager.hpp"
#include "Defaults.hpp"
#include "ActionManager.hpp"
#include "Button.hpp"
#include <iostream>

class FlowGrid : public Grid
{
public:
    FlowGrid() = delete;
    FlowGrid(u_short rows, u_short cols, u_short cellSize, std::vector<ColorNodes> colorNodes, std::vector<std::vector<bool>> cellExists, sf::RenderWindow &window);
    FlowGrid &operator=(FlowGrid &&other) noexcept;
    ~FlowGrid();

    u_short getCellSize() const override;
    u_short getGridLineThickness() const override;
    u_short getRows() const override;
    u_short getCols() const override;
    sf::Vector2f getGridSize() const override;

    bool isDrawing();
    void draw(sf::RenderWindow &window);
    sf::Vector2f getCellPos(u_short row, u_short col);
    void initializeShapes(std::vector<std::vector<bool>> &cellExists);
    void initializeButtons();
    void setCellSize(u_short cellSize) override;
    void makePath(int x, int y);
    void tracePath(Cell *cell, Cell *prevCell);
    void destroyPath();
    Cell *getCell(int row, int col);
    sf::Vector2f getOrigin();
    sf::Vector2f getSize();
    void undo();
    void redo();
    bool isCompleted();
    bool processEvent(const std::optional<sf::Event> &event, sf::RenderWindow &window, std::string &response);

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