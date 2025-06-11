#include "../headers/FlowGrid.hpp"

FlowGrid::FlowGrid(unsigned short rows, unsigned short cols, unsigned short cellSize, std::vector<ColorNodes> colorNodes, std::vector<std::vector<bool>> cellExists, sf::RenderWindow &window)
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

FlowGrid &FlowGrid::operator=(FlowGrid &&other) noexcept
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

FlowGrid::~FlowGrid()
{
    for (unsigned short row = 0; row < rows; ++row)
    {
        for (unsigned short col = 0; col < cols; ++col)
        {
            delete cells[row][col];
        }
    }
    cells.clear();
}

unsigned short FlowGrid::getCellSize() const { return cellSize; }
unsigned short FlowGrid::getGridLineThickness() const { return gridLineThickness; }
unsigned short FlowGrid::getRows() const { return rows; }
unsigned short FlowGrid::getCols() const { return cols; }
sf::Vector2f FlowGrid::getGridSize() const
{
    return sf::Vector2f(
        cols * cellSize + (cols + 1) * gridLineThickness,
        rows * cellSize + (rows + 1) * gridLineThickness);
}

bool FlowGrid::isDrawing() { return pathMaker.isPathDrawing(); }

void FlowGrid::draw(sf::RenderWindow &window)
{
    Cell *pathMakerHead = pathMaker.getLastCell();
    for (unsigned short row = 0; row < rows; ++row)
    {
        for (unsigned short col = 0; col < cols; ++col)
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

sf::Vector2f FlowGrid::getCellPos(unsigned short row, unsigned short col)
{
    return sf::Vector2f(
        col * cellSize + (col + 1) * gridLineThickness,
        row * cellSize + (row + 1) * gridLineThickness);
}

void FlowGrid::initializeShapes(std::vector<std::vector<bool>> &cellExists)
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

    for (unsigned short row = 0; row < rows; ++row)
    {
        for (unsigned short col = 0; col < cols; ++col)
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

void FlowGrid::initializeButtons()
{
    float PADDING = Defaults::BUTTON_PADDING;
    float startX = -origin.x + getGridSize().x / 2 - (3 * (100 + PADDING)) / (float)2;

    undoButton.setPosition(sf::Vector2f(-origin.x + this->getSize().x / 2 - undoButton.getSize().x - PADDING / 2, -origin.y - undoButton.getSize().y - PADDING));
    redoButton.setPosition(sf::Vector2f(-origin.x + this->getSize().x / 2 + PADDING / 2, -origin.y - returnButton.getSize().y - PADDING));
    returnButton.setSize(sf::Vector2f(this->getSize().x, Defaults::ACTION_BUTTON_SIZE.y));
    returnButton.setPosition(sf::Vector2f(-origin.x + this->getSize().x / 2 - returnButton.getSize().x / 2, -origin.y + this->getSize().y + PADDING));
}

void FlowGrid::setCellSize(unsigned short cellSize)
{
    this->cellSize = cellSize;
    for (unsigned short row = 0; row < rows; ++row)
    {
        for (unsigned short col = 0; col < cols; ++col)
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
}

void FlowGrid::makePath(int x, int y)
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
        unsigned short x_pad = std::abs(static_cast<int>((cellSize + gridLineThickness) / 2 - x % (cellSize + gridLineThickness)));
        unsigned short y_pad = std::abs(static_cast<int>((cellSize + gridLineThickness) / 2 - y % (cellSize + gridLineThickness)));
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

        // Get the full path from the previous cell to the current cell
        tracePath(cell, prevCell);

        // If the action is successful and a path is completed, add the path to the action manager
        if (pathMaker.addCells(pathTemp) && !pathMaker.isPathDrawing())
        {
            actionManager.undoAddAction(Action(Action::ADD, pathMaker.getPath(prevCell->getColor().toInteger())));
        }
    }
    else if (cell->colorNode)
    {
        // Start a new path
        pathMaker.startPath(cell, cell->getColor());
    }
}

void FlowGrid::tracePath(Cell *cell, Cell *prevCell)
{
    if (cell->row == prevCell->row && cell->col == prevCell->col)
    {
        pathTemp.push_back(cell);
        return;
    }
    if (cell->row == prevCell->row)
    {
        int step = (cell->col - prevCell->col) / std::abs(static_cast<int>(cell->col - prevCell->col));
        for (int c = prevCell->col + step; c != cell->col; c += step)
        {
            pathTemp.push_back(cells[cell->row][c]);
        }
    }
    else if (cell->col == prevCell->col)
    {
        int step = (cell->row - prevCell->row) / std::abs(static_cast<int>(cell->row - prevCell->row));
        for (int r = prevCell->row + step; r != cell->row; r += step)
        {
            pathTemp.push_back(cells[r][cell->col]);
        }
    }
    pathTemp.push_back(cell);
}

void FlowGrid::destroyPath()
{
    pathMaker.destroyPath();
}

Cell *FlowGrid::getCell(int row, int col)
{
    if (row >= rows || col >= cols)
    {
        std::cerr << "Error: Cell out of bounds." << std::endl;
        return nullptr;
    }
    return cells[row][col];
}

sf::Vector2f FlowGrid::getOrigin()
{
    return origin;
}

sf::Vector2f FlowGrid::getSize()
{
    return sf::Vector2f(
        cols * cellSize + (cols + 1) * gridLineThickness,
        rows * cellSize + (rows + 1) * gridLineThickness);
}

void FlowGrid::undo()
{
    actionManager.undo();
}

void FlowGrid::redo()
{
    actionManager.redo();
}

bool FlowGrid::isCompleted()
{
    return remainingSources == 0;
}

bool FlowGrid::processEvent(const std::optional<sf::Event> &event, sf::RenderWindow &window, std::string &response)
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