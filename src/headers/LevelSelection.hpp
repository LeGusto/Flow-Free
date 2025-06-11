#pragma once

#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include <algorithm>

class Level
{
public:
    Level();
    Level(Level &&other) noexcept;
    ~Level();

    void setCompleted(bool done);
    void draw(sf::RenderWindow &window);
    Button &getButton();

public:
    Button button = makeLevelButton();
    bool completed;
};

class LevelSelection
{
public:
    LevelSelection(sf::RenderWindow &window);
    ~LevelSelection();

    void calculateLevelCount();
    unsigned short getLevelCount() const;
    void handleClick(const std::optional<sf::Event> &event, std::string &response);
    void setLevelCompleted(unsigned short level);
    void draw(sf::RenderWindow &window);

private:
    sf::RectangleShape menu;
    Button returnButton = makeReturnButton();
    std::vector<Level> levelButtons;
    unsigned short levelCount;
};