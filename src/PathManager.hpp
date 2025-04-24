#pragma once
#include <vector>
#include <algorithm>
#include <map>
#include "Cells.hpp"

struct Path
{
    std::vector<Cell *> cells = {};
    std::vector<sf::Vertex> line = {};
    sf::Color color;
    sf::Vector2f *origin = nullptr;

    Path() = default;
    Path(sf::Vector2f *origin, sf::Color color) : origin(origin), color(color) {}

    void drawPath(sf::RenderWindow &window)
    {
        window.draw(&line[0], line.size(), sf::PrimitiveType::TriangleStrip);
    }

    void shrinkPath()
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

    void extendLine(Cell *cell, Cell *prevCell)
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

    void extendPath(Cell *cell)
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

    void clearPath()
    {
        while (!cells.empty())
        {
            shrinkPath();
        }
    }
    void resetPath()
    {
        cells.back()->setOutlineColor(Defaults::OUTLINE_COLOR);
        cells.clear();
        line.clear();
    }

    // Update cell path property for all cells in the path
    void finalizePath()
    {
        for (auto &cell : cells)
        {

            cell->path = color.toInteger();
            cell->setColor(color);
            cell->setOutlineColor(Defaults::OUTLINE_COLOR);
        }
    }

    void swapPath(Path &path)
    {
        this->cells = path.cells;
        this->line = path.line;
        finalizePath();
    }
};

class PathMaker
{
public:
    PathMaker() = delete; // Must supply origin
    PathMaker(sf::Vector2f *origin) : origin(origin) {}

    void startPath(Cell *startCell, sf::Color color)
    {

        if (allPaths.find(startCell->getColor().toInteger()) == allPaths.end())
        {
            allPaths[startCell->getColor().toInteger()] = Path(origin, startCell->color);
        }
        currPath = &(allPaths[startCell->getColor().toInteger()]);
        currPath->clearPath();

        currPath->origin = origin;
        currPath->color = color;
        isDrawing = true;
        currPath->extendPath(startCell);
    }
    ~PathMaker()
    {
        currPath = nullptr;
        origin = nullptr;
    }

    // Called when a path is completed (reached an end source)
    void completePath()
    {
        isDrawing = false;
        currPath->finalizePath();
        currPath = nullptr;
        isDrawing = false;
    }

    bool validatePath(std::vector<Cell *> &cellPath)
    {
        if (cellPath.size() == 0)
            return false;

        Cell *lastCell = currPath->cells.back();

        // Current endpoint of path is the same as the last cell in the path
        if (lastCell == cellPath.back())
        {
            return false;
        }

        int rStep = lastCell->row - cellPath[0]->row;
        int cStep = lastCell->col - cellPath[0]->col;

        if (abs(rStep) > 0 && abs(cStep) > 0)
            return false;

        for (int i = 0; i < cellPath.size(); ++i)
        {
            if ((rStep == 0 && cellPath[i]->row != lastCell->row) || (cStep == 0 && cellPath[i]->col != lastCell->col))
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
        }
        return true;
    }

    bool traceBackPath(Cell *cell)
    {
        Cell *lastCell = currPath->cells.back();

        // If the cell is already part of the path, shrink it down
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

    bool addCells(std::vector<Cell *> &cells)
    {
        if (!isDrawing)
        {
            return false;
        }

        // Shrink the path if the cell is already part of it
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

    bool isPathDrawing() const
    {
        return isDrawing;
    }

    Cell *getLastCell() const
    {
        if (currPath == nullptr)
            return nullptr;

        if (currPath->cells.empty())
            return nullptr;
        return currPath->cells.back();
    }

    void destroyPath()
    {
        currPath->clearPath();
        isDrawing = false;
    }

    Path *getPath() const
    {
        return currPath;
    }
    Path *getPath(int id)
    {
        return &allPaths[id];
    }

    void drawPaths(sf::RenderWindow &window)
    {
        for (auto &[key, p] : allPaths)
        {
            p.drawPath(window);
        }
    }

private:
    bool isDrawing = false;
    sf::Vector2f *origin = nullptr;
    Path *currPath = nullptr;
    std::map<int, Path> allPaths = {};
};