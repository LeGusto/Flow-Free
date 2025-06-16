#include "../headers/MainMenu.hpp"

// Initializes the main menu UI elements such as the title, buttons, and menu background
MainMenu::MainMenu(sf::RenderWindow &window)
{
    // Load the font for the title and buttons
    if (!font.openFromFile(Defaults::HOME_DIR + "/src/ARIAL.TTF"))
    {
        std::cerr << "Error loading font" << std::endl;
    }

    // Set up the menu background
    menu.setSize(sf::Vector2f(500, 300));
    menu.setFillColor(sf::Color(200, 200, 200));
    menu.setPosition(sf::Vector2f((float)window.getSize().x / 2 - menu.getSize().x / 2,
                                  (float)window.getSize().y / 2 - menu.getSize().y / 2)); // Center the menu in the window

    // Calculate the horizontal center of the menu
    float xCenter = menu.getPosition().x + menu.getSize().x / 2 -
                    startButton.getSize().x / 2;

    // Set up the title text
    title.setFont(font);
    title.setString("Flow Free");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(xCenter,
                                   menu.getPosition().y + 20)); // Position the title near the top of the menu

    // Set up the "Start" button
    startButton.setPosition(sf::Vector2f(xCenter - startButton.getSize().x / 2,
                                         menu.getPosition().y + menu.getSize().y - 100)); // Position the button near the bottom-left of the menu

    // Set up the "Exit" button
    exitButton.setPosition(sf::Vector2f(xCenter + exitButton.getSize().x / 2 + 20,
                                        menu.getPosition().y + menu.getSize().y - 100)); // Position the button near the bottom-right of the menu
}

MainMenu::~MainMenu() = default;

// Handles mouse click events on the main menu
// Updates the response string based on which button is clicked
void MainMenu::handleClick(const std::optional<sf::Event> &event, std::string &response)
{
    // Check if a mouse button was pressed
    if (const auto *mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
    {
        // Check if the left mouse button was clicked
        if (mouseButtonPressed->button == sf::Mouse::Button::Left)
        {
            // Check if the "Start" button was clicked
            if (startButton.isClicked(mouseButtonPressed->position))
            {
                response = "Start";
            }
            // Check if the "Exit" button was clicked
            else if (exitButton.isClicked(mouseButtonPressed->position))
            {
                response = "Close";
            }
            // No button was clicked
            else
            {
                response = "";
            }
        }
    }
}

// Draws the main menu elements on the SFML render window
void MainMenu::draw(sf::RenderWindow &window)
{
    window.draw(title);
    startButton.draw(window);
    exitButton.draw(window);
}