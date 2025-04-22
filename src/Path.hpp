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
    Path(sf::Vector2f *origin) : origin(origin) {}

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
        if (!cells.empty())
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
    }

    void extendPath(Cell *cell)
    {
        Cell *prevCell = nullptr;

        if (!cells.empty())
        {
            prevCell = cells.back();
            prevCell->setOutlineColor(sf::Color(128, 128, 128));
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
};

class PathMaker
{
public:
    PathMaker() = delete; // Must supply origin
    PathMaker(sf::Vector2f *origin) : origin(origin) {}

    void startPath(Cell *startCell, sf::Color color)
    {
        // Check if the cell is already part of a path.cells and reset it
        if (startCell->path != -1)
        {
            allPaths[startCell->color.toInteger()].clearPath();
        }
        path.origin = origin;
        path.color = color;
        isDrawing = true;
        path.color = color;
        path.clearPath();
        path.extendPath(startCell);
    }

    void completePath()
    {
        isDrawing = false;
        allPaths[path.color.toInteger()] = path;
        for (auto &cell : path.cells)
        {
            cell->path = path.color.toInteger();
        }
        // Don't use clearPath, because it sets all path cells to black
        path.resetPath();
    }

    bool addCell(Cell *cell)
    {
        if (!isDrawing)
            return false;

        if (path.cells.size() > 0)
        {
            Cell *lastCell = path.cells.back();

            // If the cell is already part of the path.cells, shrink it down
            if (lastCell != cell)
            {
                if (std::find(path.cells.begin(), path.cells.end(), cell) != path.cells.end())
                {
                    while (path.cells.back() != cell)
                    {
                        path.shrinkPath();
                    }
                    return true;
                }
            }

            // Ensure the cell is adjacent to the last cell in the path.cells
            if (abs(lastCell->row - cell->row) + abs(lastCell->col - cell->col) == 1)
            {
                if (cell->colorNode)
                {
                    // If the cell is a color node, check if it matches the current color
                    // Also check if it isn't the starting cell
                    if (path.color != cell->getColor() || cell == path.cells[0])
                    {
                        return false;
                    }
                    else
                    {
                        // Path is complete
                        path.extendPath(cell);
                        completePath();
                        return true;
                    }
                }
                if (cell->getColor() == sf::Color::Black)
                {
                    path.extendPath(cell);
                    return true;
                }
            }
        }
        return false;
    }

    bool isPathDrawing() const
    {
        return isDrawing;
    }

    Cell *getLastCell() const
    {
        if (path.cells.empty())
            return nullptr;
        return path.cells.back();
    }

    void destroyPath()
    {
        path.clearPath();
        isDrawing = false;
    }

    void drawPaths(sf::RenderWindow &window)
    {
        for (auto &[key, p] : allPaths)
        {
            p.drawPath(window);
        }
        path.drawPath(window);
    }

private:
    bool isDrawing = false;
    sf::Vector2f *origin = nullptr;
    Path path = Path();
    std::map<int, Path> allPaths = {};
};