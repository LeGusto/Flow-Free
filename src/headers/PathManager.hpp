#pragma once

#include <vector>
#include <algorithm>
#include <map>
#include "Cells.hpp"

struct Path
{
    Path();
    Path(sf::Vector2f *origin, sf::Color color, u_short *remainingSources);
    ~Path();

    void drawPath(sf::RenderWindow &window);
    void shrinkPath();
    void extendLine(Cell *cell, Cell *prevCell);
    void extendPath(Cell *cell);
    void clearPath();
    void resetPath();
    void finalizePath();
    void swapPath(Path &path);

    std::vector<Cell *> cells;
    std::vector<sf::Vertex> line;
    sf::Color color;
    bool completed = false;
    sf::Vector2f *origin = nullptr;
    u_short *remainingSources = nullptr;
};

class PathMaker
{
public:
    PathMaker() = delete;
    PathMaker(sf::Vector2f *origin, u_short *remainingSources);
    ~PathMaker();

    void startPath(Cell *startCell, sf::Color color);
    void completePath();
    bool validatePath(std::vector<Cell *> &cellPath);
    bool traceBackPath(Cell *cell);
    bool addCells(std::vector<Cell *> &cells);
    bool isPathDrawing() const;
    Cell *getLastCell() const;
    void destroyPath();
    Path *getPath();
    Path *getPath(int id);
    void drawPaths(sf::RenderWindow &window);
    void updateRemainingSources(u_short *remainingSources);

private:
    bool isDrawing = false;
    sf::Vector2f *origin = nullptr;
    Path *currPath = nullptr;
    u_short *remainingSources = nullptr;
    std::map<int, Path> allPaths;
};