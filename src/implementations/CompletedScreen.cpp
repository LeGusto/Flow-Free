#include "../headers/CompletedScreen.hpp"

// Initializes the "Level Completed" screen with a semi-transparent background and a congratulatory title
CompletedScreen::CompletedScreen(sf::RenderWindow &window)
{
    // Load the font for the title
    if (!font.openFromFile(Defaults::HOME_DIR + "/src/ARIAL.TTF"))
    {
        std::cerr << "Error loading font" << std::endl;
    }

    // Set up the semi-transparent background menu
    menu.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    menu.setFillColor(sf::Color(sf::Color(0, 0, 0, 255 / 3 * 2)));
    menu.setPosition(sf::Vector2f((float)window.getSize().x / 2 - menu.getSize().x / 2,
                                  (float)window.getSize().y / 2 - menu.getSize().y / 2)); // Center the menu

    // Set up the title text
    title.setFont(font);
    title.setString("Level Completed!");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::Green);
    title.setOutlineColor(sf::Color(0, 144, 0));
    title.setOutlineThickness(2);
    title.setPosition(sf::Vector2f(menu.getPosition().x + menu.getSize().x / 2 - title.getGlobalBounds().size.x / 2,
                                   menu.getPosition().y + menu.getSize().y / 2 - title.getGlobalBounds().size.y / 2)); // Center the title
}

CompletedScreen::~CompletedScreen() = default;

// Handles mouse click events on the "Level Completed" screen
// Updates the response string to "Return" when the left mouse button is clicked
void CompletedScreen::handleClick(const std::optional<sf::Event> &event, std::string &response)
{
    // Check if a mouse button was pressed
    if (const auto *mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
    {
        // Check if the left mouse button was clicked
        if (mouseButtonPressed->button == sf::Mouse::Button::Left)
        {
            response = "Return";
        }
    }
}

// Draws the "Level Completed" screen on the SFML render window
void CompletedScreen::draw(sf::RenderWindow &window)
{
    window.draw(menu);
    window.draw(title);
}