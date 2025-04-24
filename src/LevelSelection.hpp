#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include <algorithm>

class LevelSelection
{
public:
    LevelSelection(sf::RenderWindow &window)
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
        float buttonSpacing = 10;

        float menuWidth = menu.getSize().x;
        float menuX = menu.getPosition().x;
        float menuY = menu.getPosition().y;

        float xOffset = menuX + buttonSpacing;
        float yOffset = menuY + buttonSpacing;

        for (u_short i = 0; i < levelCount; ++i)
        {
            if (xOffset + buttonWidth > menuX + menuWidth - buttonSpacing)
            {
                xOffset = menuX + buttonSpacing;
                yOffset += buttonHeight + buttonSpacing;
            }
            Button button = Button();

            button.setTextSize(20);
            button.setSize(sf::Vector2f(buttonWidth, buttonHeight));
            button.setPosition(sf::Vector2f(xOffset, yOffset));
            button.setText(("Level " + std::to_string(i + 1)));

            button.setColor(sf::Color::Black);
            button.setOutlineColor(sf::Color::White);
            button.setOutlineThickness(2);
            button.setTextColor(sf::Color::White);

            levelButtons.push_back(button);

            xOffset += buttonWidth + buttonSpacing;
        }
    };
    ~LevelSelection() = default;

    void calculateLevelCount()
    {
        auto dirIter = std::filesystem::directory_iterator("src/levels");

        u_short fileCount = std::count_if(
            begin(dirIter),
            end(dirIter),
            [](auto &entry)
            { return entry.is_regular_file(); });

        levelCount = fileCount;
    }

    u_short getLevelCount() const
    {
        return levelCount;
    }

    void handleClick(const std::optional<sf::Event> &event, std::string &response)
    {
        if (const auto *mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseButtonPressed->button == sf::Mouse::Button::Left)
            {
                response = "";
                for (u_short i = 0; i < levelButtons.size(); ++i)
                {
                    if (levelButtons[i].isClicked(mouseButtonPressed->position))
                    {
                        response = std::to_string(i + 1);
                        return;
                    }
                }
            }
        }
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(menu);
        for (auto &button : levelButtons)
        {
            button.draw(window);
        }
    };

private:
    sf::RectangleShape menu;
    std::vector<Button> levelButtons = {};
    u_short levelCount = 0;
};