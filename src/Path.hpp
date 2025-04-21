#pragma once
#include <vector>
#include <algorithm>
#include <map>
#include "Cells.hpp"

struct pathShape
{
    sf::VertexArray line;
    sf::Color color;
};

class Path
{
public:
    void resetPath(int color)
    {
        for (auto &cell : allPaths[color])
        {
            cell->setColor(sf::Color::Black);
            cell->path = -1;
        }
        allPaths.erase(color);
    }

    void shrinkPath()
    {
        if (path.size() > 0)
        {
            Cell *lastCell = path.back();
            lastCell->setColor(sf::Color::Black);
            lastCell->setOutlineColor(sf::Color(128, 128, 128));

            path.pop_back();
        }

        if (!path.empty())
        {
            path.back()->setOutlineColor(sf::Color::White);
        }
    }

    void clearPath()
    {
        while (!path.empty())
        {
            shrinkPath();
        }
        isDrawing = false;
    }

    void extendPath(Cell *cell)
    {
        if (!path.empty())
        {
            path.back()->setOutlineColor(sf::Color(128, 128, 128));
        }
        path.push_back(cell);
        cell->setColor(currentColor);
        cell->setOutlineColor(sf::Color::White);
    }

    void startPath(Cell *startCell, sf::Color color)
    {
        // Check if the cell is already part of a path and reset it
        if (startCell->path != -1)
        {
            resetPath(startCell->color.toInteger());
        }

        isDrawing = true;
        currentColor = color;
        path.clear();
        extendPath(startCell);
    }

    void completePath()
    {
        isDrawing = false;
        allPaths[currentColor.toInteger()] = path;
        for (auto &cell : path)
        {
            cell->path = currentColor.toInteger();
        }
        if (!path.empty())
            path.back()->setOutlineColor(sf::Color(128, 128, 128));
        path.clear();
    }

    bool addCell(Cell *cell)
    {
        if (!isDrawing)
            return false;

        if (path.size() > 0)
        {
            Cell *lastCell = path.back();

            // If the cell is already part of the path, shrink it down
            if (lastCell != cell)
            {
                if (std::find(path.begin(), path.end(), cell) != path.end())
                {
                    while (path.back() != cell)
                    {
                        shrinkPath();
                    }
                    return true;
                }
            }

            // Ensure the cell is adjacent to the last cell in the path
            if (abs(lastCell->row - cell->row) + abs(lastCell->col - cell->col) == 1)
            {
                if (cell->colorNode)
                {
                    // If the cell is a color node, check if it matches the current color
                    // Also check if it isn't the starting cell
                    if (currentColor != cell->getColor() || cell == path[0])
                    {
                        return false;
                    }
                    else
                    {
                        // Path is complete
                        extendPath(cell);
                        completePath();
                        return true;
                    }
                }
                if (cell->getColor() == sf::Color::Black)
                {
                    extendPath(cell);
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
        if (path.empty())
            return nullptr;
        return path.back();
    }

private:
    bool isDrawing = false;
    sf::Color currentColor = sf::Color::Black;
    std::vector<Cell *> path = {};
    std::map<int, std::vector<Cell *>> allPaths = {};
};