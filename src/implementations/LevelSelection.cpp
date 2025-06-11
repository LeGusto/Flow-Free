#include "../headers/LevelSelection.hpp"
#include <filesystem>
#include <algorithm>

Level::Level() = default;
Level::Level(Level &&other) noexcept
    : button(std::move(other.button)), completed(other.completed) {}
Level::~Level() = default;

void Level::setCompleted(bool done)
{
    completed = done;
    button.setOutlineColor(done ? sf::Color::Green : sf::Color::White);
    button.setTextColor(done ? sf::Color::Green : sf::Color::White);
}

void Level::draw(sf::RenderWindow &window)
{
    button.draw(window);
}

Button &Level::getButton()
{
    return button;
}

LevelSelection::LevelSelection(sf::RenderWindow &window)
{
    calculateLevelCount();

    menu.setSize(sf::Vector2f(500, 300));
    menu.setFillColor(sf::Color::Black);
    menu.setOutlineColor(sf::Color::White);
    menu.setOutlineThickness(2);
    menu.setPosition(sf::Vector2f((float)window.getSize().x / 2 - menu.getSize().x / 2,
                                  (float)window.getSize().y / 2 - menu.getSize().y / 2));

    float buttonWidth = 100;
    float buttonHeight = 50;
    float buttonSpacing = Defaults::BUTTON_PADDING;

    float menuWidth = menu.getSize().x;
    float menuHeight = menu.getSize().y;
    float menuX = menu.getPosition().x;
    float menuY = menu.getPosition().y;

    float xOffset = menuX + buttonSpacing;
    float yOffset = menuY + buttonSpacing;

    for (unsigned short i = 0; i < levelCount; ++i)
    {
        if (xOffset + buttonWidth > menuX + menuWidth - buttonSpacing)
        {
            xOffset = menuX + buttonSpacing;
            yOffset += buttonHeight + buttonSpacing;
        }
        Level lvl = Level();
        Button &button = lvl.getButton();

        button.setText("Level " + std::to_string(i + 1));
        button.setPosition(sf::Vector2f(xOffset, yOffset));

        levelButtons.push_back(std::move(lvl));

        xOffset += buttonWidth + buttonSpacing;
    }

    returnButton.setSize(sf::Vector2f(menuWidth, 50));
    returnButton.setPosition(sf::Vector2f(menuX + menuWidth / 2 - returnButton.getSize().x / 2,
                                          menuY + menuHeight + buttonSpacing));
}

LevelSelection::~LevelSelection() = default;

void LevelSelection::calculateLevelCount()
{
    auto dirIter = std::filesystem::directory_iterator("src/levels");

    unsigned short fileCount = std::count_if(
        begin(dirIter),
        end(dirIter),
        [](auto &entry)
        { return entry.is_regular_file(); });

    levelCount = fileCount;
}

unsigned short LevelSelection::getLevelCount() const
{
    return levelCount;
}

void LevelSelection::handleClick(const std::optional<sf::Event> &event, std::string &response)
{
    if (const auto *mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left)
        {
            response = "";
            for (unsigned short i = 0; i < levelButtons.size(); ++i)
            {
                if (levelButtons[i].getButton().isClicked(mouseButtonPressed->position))
                {
                    response = std::to_string(i + 1);
                    return;
                }
            }
            if (returnButton.isClicked(mouseButtonPressed->position))
            {
                response = "Return";
            }
        }
    }
}

void LevelSelection::setLevelCompleted(unsigned short level)
{
    if (level <= levelButtons.size())
    {
        levelButtons[level - 1].setCompleted(true);
    }
}

void LevelSelection::draw(sf::RenderWindow &window)
{
    window.draw(menu);
    for (auto &button : levelButtons)
    {
        button.draw(window);
    }
    returnButton.draw(window);
}