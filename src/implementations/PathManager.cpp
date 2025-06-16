#include "../headers/PathManager.hpp"

Path::Path() = default;

Path::Path(sf::Vector2f *origin, sf::Color color, unsigned short *remainingSources)
    : origin(origin), color(color), remainingSources(remainingSources) {}

Path::~Path() = default;

// Draws the path on the given SFML render window
void Path::drawPath(sf::RenderWindow &window)
{
    if (line.empty())
        return;
    window.draw(&line[0], line.size(), sf::PrimitiveType::TriangleStrip);
}

// Shrinks the path by removing the last cell and its corresponding vertices
void Path::shrinkPath()
{
    if (cells.size() > 0)
    {
        Cell *lastCell = cells.back();
        lastCell->setColor(sf::Color::Black);
        lastCell->setOutlineColor(Defaults::OUTLINE_COLOR);
        lastCell->path = -1;
        cells.pop_back();

        if (!line.empty())
        {
            // Remove the last 5 vertices from the line
            for (int i = 0; i < 5; ++i)
            {
                line.pop_back();
            }
        }
    }

    if (!cells.empty())
    {
        cells.back()->setOutlineColor(color);
    }
}

// Extends the path by adding vertices between the current cell and the previous cell
void Path::extendLine(Cell *cell, Cell *prevCell)
{
    // Create vertices for the line segment
    sf::Vertex vertexUp1{
        sf::Vector2f(-origin->x + prevCell->shape.getPosition().x + cell->shape.getSize().x / 2,
                     -origin->y + prevCell->shape.getPosition().y + cell->shape.getSize().y / 2),
        color};
    sf::Vertex vertexDown1 = vertexUp1;
    sf::Vertex vertexUp2{
        sf::Vector2f(-origin->x + cell->shape.getPosition().x + cell->shape.getSize().x / 2,
                     -origin->y + cell->shape.getPosition().y + cell->shape.getSize().y / 2),
        color};
    sf::Vertex vertexDown2 = vertexUp2;
    sf::Vertex mid = vertexUp1;

    sf::Vector2f mod = {0, 0};

    // Adjust vertices based on the direction of the path
    if (prevCell->col == cell->col)
    {
        mid.position.y += cell->shape.getSize().y * (Defaults::PATH_THICKNESS * -(cell->row - prevCell->row));
        mod.x += cell->shape.getSize().x * Defaults::PATH_THICKNESS;
    }
    else
    {
        mid.position.x += cell->shape.getSize().x * (Defaults::PATH_THICKNESS * -(cell->col - prevCell->col));
        mod.y += cell->shape.getSize().y * Defaults::PATH_THICKNESS;
    }

    // Adjust vertex positions for thickness
    vertexDown1.position.x -= mod.x;
    vertexDown1.position.y -= mod.y;
    vertexUp1.position.x += mod.x;
    vertexUp1.position.y += mod.y;
    vertexDown2.position.x -= mod.x;
    vertexDown2.position.y -= mod.y;
    vertexUp2.position.x += mod.x;
    vertexUp2.position.y += mod.y;

    // Swap vertices for alternating rows
    if (cells.size() & 1)
    {
        std::swap(vertexUp1, vertexDown1);
        std::swap(vertexUp2, vertexDown2);
        std::swap(vertexDown2, vertexUp2);
        std::swap(vertexUp1, vertexDown1);
    }

    // Add vertices to the line
    line.push_back(std::move(mid));
    line.push_back(std::move(vertexUp1));
    line.push_back(std::move(vertexDown1));
    line.push_back(std::move(vertexUp2));
    line.push_back(std::move(vertexDown2));
}

// Extends the path by adding a new cell
void Path::extendPath(Cell *cell)
{
    Cell *prevCell = nullptr;

    // Remove the header outline color from the previous cell
    if (!cells.empty())
    {
        prevCell = cells.back();
        prevCell->setOutlineColor(Defaults::OUTLINE_COLOR);
    }

    cells.push_back(cell);
    cell->path = color.toInteger();
    cell->setOutlineColor(color);
    cell->setColor(color);

    if (prevCell != nullptr)
        extendLine(cell, prevCell);
}

// Clears the entire path
void Path::clearPath()
{
    while (!cells.empty())
    {
        shrinkPath();
    }

    // Update the remaining sources if this path was completed before ckearing
    if (remainingSources != nullptr && completed)
    {
        ++(*remainingSources);
        completed = false;
    }
    completed = false;
}

// Resets the path to its initial state
void Path::resetPath()
{
    cells.back()->setOutlineColor(Defaults::OUTLINE_COLOR);
    cells.clear();
    line.clear();
    completed = false;
}

// Finalizes the path, marking it as completed
void Path::finalizePath()
{
    for (auto &cell : cells)
    {
        cell->path = color.toInteger();
        cell->setColor(color);
        cell->setOutlineColor(Defaults::OUTLINE_COLOR);
    }
    if (remainingSources != nullptr && !completed)
    {
        --(*remainingSources);
        completed = true;
    }
}

// Swaps the current path with another path (for undo/redo functionality)
void Path::swapPath(Path &path)
{
    this->cells = path.cells;
    this->line = path.line;
    finalizePath();
}

// -----------------------------------------------------------------------------

PathMaker::PathMaker(sf::Vector2f *origin, unsigned short *remainingSources)
    : origin(origin), remainingSources(remainingSources) {}

PathMaker::~PathMaker()
{
    currPath = nullptr;
    origin = nullptr;
}

// Starts a new path from the given starting cell
void PathMaker::startPath(Cell *startCell, sf::Color color)
{
    if (allPaths.find(startCell->getColor().toInteger()) == allPaths.end())
    {
        allPaths[startCell->getColor().toInteger()] = Path(origin, startCell->color, remainingSources);
    }
    currPath = &(allPaths[startCell->getColor().toInteger()]);
    currPath->clearPath();

    currPath->origin = origin;
    currPath->color = color;
    isDrawing = true;
    currPath->extendPath(startCell);
}

// Completes the current path
void PathMaker::completePath()
{
    isDrawing = false;
    currPath->finalizePath();
    currPath = nullptr;
    isDrawing = false;
}

// Validates the given path based on game rules
bool PathMaker::validatePath(std::vector<Cell *> &cellPath)
{
    if (cellPath.size() == 0)
        return false;

    Cell *lastCell = currPath->cells.back();

    if (lastCell == cellPath.back())
    {
        return false;
    }

    short rStep = (int)lastCell->row - cellPath[0]->row;
    short cStep = (int)lastCell->col - cellPath[0]->col;
    unsigned short prevR = lastCell->row;
    unsigned short prevC = lastCell->col;

    if (std::abs(static_cast<int>(rStep)) > 0 && std::abs(static_cast<int>(cStep)) > 0)
        return false;

    for (int i = 0; i < cellPath.size(); ++i)
    {
        if ((rStep == 0 && cellPath[i]->row != lastCell->row) || (cStep == 0 && cellPath[i]->col != lastCell->col))
        {
            return false;
        }
        if (std::abs((int)cellPath[i]->row - prevR) != 1 && std::abs((int)cellPath[i]->col - prevC) != 1)
        {
            return false;
        }
        if (cellPath[i]->getColor() != Defaults::CELL_COLOR && (cellPath[i]->colorNode == false || cellPath[i]->getColor() != lastCell->getColor()))
        {
            return false;
        }
        if (cellPath[i]->color == lastCell->color && cellPath[i]->colorNode)
        {
            return false;
        }

        if (cellPath[i]->colorNode)
        {
            while (cellPath.back() != cellPath[i])
            {
                cellPath.pop_back();
            }
        }

        prevR = cellPath[i]->row;
        prevC = cellPath[i]->col;
    }
    return true;
}

// Traces back the path to a specific cell
bool PathMaker::traceBackPath(Cell *cell)
{
    Cell *lastCell = currPath->cells.back();

    if (lastCell != cell)
    {
        if (std::find(currPath->cells.begin(), currPath->cells.end(), cell) != currPath->cells.end())
        {
            while (currPath->cells.back() != cell)
            {
                currPath->shrinkPath();
            }
            return true;
        }
    }
    return false;
}

// Adds cells to the current path
bool PathMaker::addCells(std::vector<Cell *> &cells)
{
    if (!isDrawing)
    {
        return false;
    }

    if (traceBackPath(cells.back()))
    {
        return false;
    }

    if (!validatePath(cells))
    {
        return false;
    }

    for (Cell *cell : cells)
    {
        currPath->extendPath(cell);
        if (cell->colorNode)
        {
            completePath();
        }
    }
    return true;
}

// Checks if a path is currently being drawn
bool PathMaker::isPathDrawing() const
{
    return isDrawing;
}

// Gets the last cell in the current path
Cell *PathMaker::getLastCell() const
{
    if (currPath == nullptr)
        return nullptr;

    if (currPath->cells.empty())
        return nullptr;
    return currPath->cells.back();
}

// Destroys the current path
void PathMaker::destroyPath()
{
    currPath->clearPath();
    isDrawing = false;
}

// Gets the current path
Path *PathMaker::getPath()
{
    return currPath;
}

// Gets a path by its ID
Path *PathMaker::getPath(int id)
{
    return &allPaths[id];
}

// Draws all paths on the given SFML render window
void PathMaker::drawPaths(sf::RenderWindow &window)
{
    for (auto &[key, p] : allPaths)
    {
        p.drawPath(window);
    }
}

// Updates the remaining sources for all paths
void PathMaker::updateRemainingSources(unsigned short *remainingSources)
{
    this->remainingSources = remainingSources;
    for (auto &[key, p] : allPaths)
    {
        p.remainingSources = remainingSources;
    }
}