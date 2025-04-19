#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

class Grid {
    public:

        Grid(u_short rows, u_short cols, u_short cellSize)
        : rows(rows), cols(cols), cellSize(cellSize) {}

        virtual ~Grid() = default;

        virtual void setCellSize(u_short cellSize) = 0;
        virtual void setCellColor(u_short row, u_short col, const sf::Color& color) = 0;

        virtual u_short getCellSize() const = 0;
        virtual u_short getGridLineThickness() const = 0;
        virtual u_short getRows() const = 0;
        virtual u_short getCols() const = 0; 
        
    private:
        u_short rows = 0;
        u_short cols = 0;
        u_short cellSize = 0;
        u_short gridLineThickness = 0;
};