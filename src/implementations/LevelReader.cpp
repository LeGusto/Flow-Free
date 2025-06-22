#include "../headers/LevelReader.hpp"
#include <iostream>
#include <fstream>

// Map of characters to SFML colors for parsing level files
std::unordered_map<char, sf::Color> colorMap = {
    {'R', sf::Color::Red},
    {'G', sf::Color::Green},
    {'B', sf::Color::Blue},
    {'Y', sf::Color::Yellow},
    {'M', sf::Color::Magenta},
    {'C', sf::Color::Cyan},
    {'O', sf::Color(255, 165, 0)}, // Orange
    {'P', sf::Color(128, 0, 128)}  // Purple
};

// Vector to store color nodes (special cells with colors)
std::vector<ColorNodes> colorNodes;

// 2D vector to track whether a cell exists at a given position
std::vector<std::vector<bool>> cellExists;

// Verifies that each color node appears exactly twice in the grid
bool verifyColorNodes()
{
    std::unordered_map<int, int> colorCount;

    for (const auto &node : colorNodes)
    {
        int colorKey = node.color.toInteger(); // Use the color's integer representation as a key
        ++colorCount[colorKey];                // Count occurrences of each color
    }

    for (auto [key, count] : colorCount)
    {
        // Return false if any color appears more than once
        if (count != 2)
        {
            std::cerr << "Error: A color appears more than once." << std::endl;
            return false;
        }
    }

    return true;
}

// Reads a level file and constructs a FlowGrid object
FlowGrid readLevel(int level, sf::RenderWindow &window)
{
    // Clear previous level data
    colorNodes.clear();
    cellExists.clear();

    // Open the level file
    std::ifstream file(Defaults::HOME_DIR + "/src/levels/base/" + std::to_string(level) + ".txt");
    if (!file.is_open())
    {
        // Handle file open error
        std::cerr << "Error: Could not open level file." << std::endl;
        return FlowGrid(0, 0, 0, colorNodes, cellExists, window); // Return an empty FlowGrid
    }

    unsigned short rows, cols;

    // Read the number of rows and columns from the file
    file >> rows >> cols;

    // Resize the cellExists vector to match the grid dimensions
    cellExists.resize(rows, std::vector<bool>(cols, false));

    // Parse the level file to populate the grid
    for (unsigned short row = 0; row < rows; ++row)
    {
        for (unsigned short col = 0; col < cols; ++col)
        {
            char cellType;
            file >> cellType; // Read the cell type

            if (cellType == '#')
            {
                // '#' represents a blocking cell
                cellExists[row][col] = true;
            }
            else if (colorMap.find(cellType) != colorMap.end())
            {
                // If the cell type is a color, add it to the colorNodes vector
                sf::Color color = colorMap[cellType];
                colorNodes.push_back({row, col, color});
                cellExists[row][col] = true;
            }
        }

        // Ignore the newline character at the end of the row
        if (file.peek() == '\n')
        {
            file.ignore();
        }
    }

    if (!verifyColorNodes())
    {

        throw std::runtime_error("Invalid level file: color nodes verification failed.");
    }

    // Return a FlowGrid object initialized with the parsed data
    return FlowGrid(rows, cols, Defaults::CELL_SIZE, colorNodes, cellExists, window);
}