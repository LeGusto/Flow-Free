
#include <iostream>
#include "Button.hpp"
#include <SFML/Graphics.hpp>

class CompletedScreen
{
public:
    CompletedScreen(sf::RenderWindow &window)
    {
        if (!font.openFromFile("src/ARIAL.TTF"))
        {
            std::cerr << "Error loading font" << std::endl;
        }

        menu.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        menu.setFillColor(sf::Color(sf::Color(0, 0, 0, 255 / 3 * 2)));
        menu.setPosition(sf::Vector2f((float)window.getSize().x / 2 - menu.getSize().x / 2,
                                      (float)window.getSize().y / 2 - menu.getSize().y / 2));

        title.setFont(font);
        title.setString("Level Completed!");
        title.setCharacterSize(50);
        title.setFillColor(sf::Color::Green);
        title.setOutlineColor(sf::Color(0, 144, 0));
        title.setOutlineThickness(2);
        title.setPosition(sf::Vector2f(menu.getPosition().x + menu.getSize().x / 2 - title.getGlobalBounds().size.x / 2,
                                       menu.getPosition().y + menu.getSize().y / 2 - title.getGlobalBounds().size.y / 2));

        // returnButton.setSize(sf::Vector2f(100, 50));
        // returnButton.setPosition(sf::Vector2f(menu.getPosition().x + menu.getSize().x / 2 - returnButton.getSize().x / 2,
        //                                       menu.getPosition().y + menu.getSize().y - 100));
        // returnButton.setText("Return");
        // returnButton.setTextColor(sf::Color::White);
    };
    ~CompletedScreen() = default;

    void display()
    {
        // Code to display the completed screen
        std::cout << "Congratulations! You've completed the game!" << std::endl;
    }

    void reset()
    {
        // Code to reset the completed screen
        std::cout << "Resetting completed screen..." << std::endl;
    }
    void handleClick(const std::optional<sf::Event> &event, std::string &response)
    {
        if (const auto *mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseButtonPressed->button == sf::Mouse::Button::Left)
            {
                // if (returnButton.isClicked(mouseButtonPressed->position))
                // {
                response = "Return";
                // }
            }
        }
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(menu);
        window.draw(title);
        // returnButton.draw(window);
    }

private:
    sf::RectangleShape menu;
    sf::Font font;
    sf::Text title = sf::Text(font);
    // Button returnButton = Button();
};