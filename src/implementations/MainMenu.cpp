#include "../headers/MainMenu.hpp"

MainMenu::MainMenu(sf::RenderWindow &window)
{
    if (!font.openFromFile(Defaults::HOME_DIR + "/src/ARIAL.TTF"))
    {
        std::cerr << "Error loading font" << std::endl;
    }

    menu.setSize(sf::Vector2f(500, 300));
    menu.setFillColor(sf::Color(200, 200, 200));
    menu.setPosition(sf::Vector2f((float)window.getSize().x / 2 - menu.getSize().x / 2,
                                  (float)window.getSize().y / 2 - menu.getSize().y / 2));

    float xCenter = menu.getPosition().x + menu.getSize().x / 2 -
                    startButton.getSize().x / 2;

    title.setFont(font);
    title.setString("Flow Free");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(xCenter,
                                   menu.getPosition().y + 20));

    // startButton = makeStartButton();
    // exitButton = makeExitButton();

    startButton.setPosition(sf::Vector2f(xCenter - startButton.getSize().x / 2,
                                         menu.getPosition().y + menu.getSize().y - 100));

    exitButton.setPosition(sf::Vector2f(xCenter + exitButton.getSize().x / 2 + 20,
                                        menu.getPosition().y + menu.getSize().y - 100));
}

MainMenu::~MainMenu() = default;

void MainMenu::handleClick(const std::optional<sf::Event> &event, std::string &response)
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

void MainMenu::draw(sf::RenderWindow &window)
{
    window.draw(title);
    // std::cout << startButton.getSize().x << " " << startButton.getSize().y << std::endl;
    startButton.draw(window);
    exitButton.draw(window);
}