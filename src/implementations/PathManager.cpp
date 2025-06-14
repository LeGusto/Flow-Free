#include "../headers/PathManager.hpp"

Path::Path() = default;
Path::Path(sf::Vector2f *origin, sf::Color color, unsigned short *remainingSources) : origin(origin), color(color), remainingSources(remainingSources) {}
Path::~Path() = default;

void Path::drawPath(sf::RenderWindow &window)
{
    if (line.empty())
        return;
    window.draw(&line[0], line.size(), sf::PrimitiveType::TriangleStrip);
}

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

void Path::extendLine(Cell *cell, Cell *prevCell)
{
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
    vertexDown1.position.x -= mod.x;
    vertexDown1.position.y -= mod.y;
    vertexUp1.position.x += mod.x;
    vertexUp1.position.y += mod.y;
    vertexDown2.position.x -= mod.x;
    vertexDown2.position.y -= mod.y;
    vertexUp2.position.x += mod.x;
    vertexUp2.position.y += mod.y;
    if (cells.size() & 1)
    {
        std::swap(vertexUp1, vertexDown1);
        std::swap(vertexUp2, vertexDown2);
        std::swap(vertexDown2, vertexUp2);
        std::swap(vertexUp1, vertexDown1);
    }
    line.push_back(std::move(mid));
    line.push_back(std::move(vertexUp1));
    line.push_back(std::move(vertexDown1));
    line.push_back(std::move(vertexUp2));
    line.push_back(std::move(vertexDown2));
}

void Path::extendPath(Cell *cell)
{
    Cell *prevCell = nullptr;

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

void Path::clearPath()
{
    while (!cells.empty())
    {
        shrinkPath();
    }
    if (remainingSources != nullptr && completed)
    {
        ++(*remainingSources);
        completed = false;
    }
    completed = false;
}

void Path::resetPath()
{
    cells.back()->setOutlineColor(Defaults::OUTLINE_COLOR);
    cells.clear();
    line.clear();
    completed = false;
}

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

void Path::swapPath(Path &path)
{
    this->cells = path.cells;
    this->line = path.line;
    finalizePath();
}

PathMaker::PathMaker(sf::Vector2f *origin, unsigned short *remainingSources) : origin(origin), remainingSources(remainingSources) {}
PathMaker::~PathMaker()
{
    currPath = nullptr;
    origin = nullptr;
}

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

void PathMaker::completePath()
{
    isDrawing = false;
    currPath->finalizePath();
    currPath = nullptr;
    isDrawing = false;
}

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

bool PathMaker::isPathDrawing() const
{
    return isDrawing;
}

Cell *PathMaker::getLastCell() const
{
    if (currPath == nullptr)
        return nullptr;

    if (currPath->cells.empty())
        return nullptr;
    return currPath->cells.back();
}

void PathMaker::destroyPath()
{
    currPath->clearPath();
    isDrawing = false;
}

Path *PathMaker::getPath()
{
    return currPath;
}

Path *PathMaker::getPath(int id)
{
    return &allPaths[id];
}

void PathMaker::drawPaths(sf::RenderWindow &window)
{
    for (auto &[key, p] : allPaths)
    {
        p.drawPath(window);
    }
}

void PathMaker::updateRemainingSources(unsigned short *remainingSources)
{
    this->remainingSources = remainingSources;
    for (auto &[key, p] : allPaths)
    {
        p.remainingSources = remainingSources;
    }
}