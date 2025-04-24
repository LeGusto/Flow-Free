#include "Button.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>

class MainMenu
{
public:
    MainMenu(sf::RenderWindow &window)
    {
        if (!font.openFromFile("src/ARIAL.TTF"))
        {
            std::cerr << "Error loading font" << std::endl;
        }

        menu.setSize(sf::Vector2f(500, 300));
        menu.setFillColor(sf::Color(200, 200, 200));
        menu.setPosition(sf::Vector2f((float)window.getSize().x / 2 - menu.getSize().x / 2,
                                      (float)window.getSize().y / 2 - menu.getSize().y / 2));

        startButton.setSize(Defaults::MENU_BUTTON_SIZE);
        exitButton.setSize(Defaults::MENU_BUTTON_SIZE);
        float xCenter = menu.getPosition().x + menu.getSize().x / 2 -
                        startButton.getSize().x / 2;

        title.setFont(font);
        title.setString("Flow Free");
        title.setCharacterSize(50);
        title.setFillColor(sf::Color::White);
        // title.setOutlineColor("")
        title.setPosition(sf::Vector2f(xCenter,
                                       menu.getPosition().y + 20));

        startButton.setPosition(sf::Vector2f(xCenter - startButton.getSize().x / 2 - 20,
                                             menu.getPosition().y + menu.getSize().y - 100));
        startButton.setColor(sf::Color::Black);
        startButton.setOutlineColor(sf::Color::White);
        startButton.setOutlineThickness(2);
        startButton.setText("Start");
        startButton.setTextColor(sf::Color::White);

        exitButton.setPosition(sf::Vector2f(xCenter + exitButton.getSize().x / 2 + 20,
                                            menu.getPosition().y + menu.getSize().y - 100));
        exitButton.setColor(sf::Color::Black);
        exitButton.setOutlineColor(sf::Color::White);
        exitButton.setOutlineThickness(2);
        exitButton.setText("Exit");
        exitButton.setTextColor(sf::Color::White);
    };
    ~MainMenu() = default;

    void handleClick(const std::optional<sf::Event> &event, std::string &response)
    {
        if (const auto *mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseButtonPressed->button == sf::Mouse::Button::Left)
            {
                if (startButton.isClicked(mouseButtonPressed->position))
                {
                    response = "Start";
                }
                else if (exitButton.isClicked(mouseButtonPressed->position))
                {
                    response = "Close";
                }
                else
                {
                    response = "";
                }
            }
        }
    }

    void draw(sf::RenderWindow &window)
    {
        // window.draw(menu);
        window.draw(title);
        startButton.draw(window);
        exitButton.draw(window);
    }

private:
    sf::RectangleShape menu;
    sf::Font font;
    sf::Text title = sf::Text(font);
    Button startButton = Button();
    Button exitButton = Button();
};