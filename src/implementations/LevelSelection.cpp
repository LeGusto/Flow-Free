#include "../headers/LevelSelection.hpp"
#include <filesystem>
#include <algorithm>

Level::Level() = default;

// Move constructor for Level
Level::Level(Level &&other) noexcept
    : button(std::move(other.button)), completed(other.completed) {}

Level::~Level() = default;

// Marks the level as completed or not and updates the button's appearance
void Level::setCompleted(bool done)
{
    completed = done;
    button.setOutlineColor(done ? sf::Color::Green : sf::Color::White); // Green for completed, white otherwise
    button.setTextColor(done ? sf::Color::Green : sf::Color::White);    // Update text color accordingly
}

// Draws the level button on the SFML render window
void Level::draw(sf::RenderWindow &window)
{
    button.draw(window);
}

// Returns a reference to the button associated with the level
Button &Level::getButton()
{
    return button;
}

// Initializes the level selection menu and dynamically creates buttons for each level
LevelSelection::LevelSelection(sf::RenderWindow &window)
{
    calculateLevelCount(); // Calculate the total number of levels

    // Set up the menu background
    menu.setSize(sf::Vector2f(500, 300));
    menu.setFillColor(sf::Color::Black);
    menu.setOutlineColor(sf::Color::White);
    menu.setOutlineThickness(2);
    menu.setPosition(sf::Vector2f((float)window.getSize().x / 2 - menu.getSize().x / 2,
                                  (float)window.getSize().y / 2 - menu.getSize().y / 2)); // Center the menu in the window

    // Button dimensions and spacing
    float buttonWidth = 100;
    float buttonHeight = 50;
    float buttonSpacing = Defaults::BUTTON_PADDING;

    // Menu dimensions and position
    float menuWidth = menu.getSize().x;
    float menuHeight = menu.getSize().y;
    float menuX = menu.getPosition().x;
    float menuY = menu.getPosition().y;

    // Initial offsets for button placement
    float xOffset = menuX + buttonSpacing;
    float yOffset = menuY + buttonSpacing;

    // Create buttons for each level
    for (unsigned short i = 0; i < levelCount; ++i)
    {
        // Move to the next row if the button exceeds the menu width
        if (xOffset + buttonWidth > menuX + menuWidth - buttonSpacing)
        {
            xOffset = menuX + buttonSpacing;         // Reset xOffset
            yOffset += buttonHeight + buttonSpacing; // Move to the next row
        }

        Level lvl = Level();              // Create a new level
        Button &button = lvl.getButton(); // Get the button associated with the level

        button.setText("Level " + std::to_string(i + 1));   // Set the button text
        button.setPosition(sf::Vector2f(xOffset, yOffset)); // Position the button

        levelButtons.push_back(std::move(lvl)); // Add the level to the list

        xOffset += buttonWidth + buttonSpacing; // Move to the next button position
    }

    // Set up the "Return" button
    returnButton.setSize(sf::Vector2f(menuWidth, 50)); // Set the size of the return button
    returnButton.setPosition(sf::Vector2f(menuX + menuWidth / 2 - returnButton.getSize().x / 2,
                                          menuY + menuHeight + buttonSpacing)); // Position the return button
}

LevelSelection::~LevelSelection() = default;

// Calculates the total number of levels by counting files in the "src/levels" directory
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

// Returns the total number of levels
unsigned short LevelSelection::getLevelCount() const
{
    return levelCount;
}

// Handles mouse click events in the level selection menu
// Updates the response string based on which button is clicked
void LevelSelection::handleClick(const std::optional<sf::Event> &event, std::string &response)
{
    if (const auto *mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) // Check if the left mouse button was clicked
        {
            response = ""; // Reset the response
            for (unsigned short i = 0; i < levelButtons.size(); ++i)
            {
                // Check if a level button was clicked
                if (levelButtons[i].getButton().isClicked(mouseButtonPressed->position))
                {
                    response = std::to_string(i + 1); // Set the response to the level number
                    return;
                }
            }
            // Check if the "Return" button was clicked
            if (returnButton.isClicked(mouseButtonPressed->position))
            {
                response = "Return";
            }
        }
    }
}

// Marks a specific level as completed
void LevelSelection::setLevelCompleted(unsigned short level)
{
    if (level <= levelButtons.size()) // Ensure the level index is valid
    {
        levelButtons[level - 1].setCompleted(true); // Mark the level as completed
    }
}

// Draws the level selection menu and all its buttons on the SFML render window
void LevelSelection::draw(sf::RenderWindow &window)
{
    window.draw(menu); // Draw the menu background
    for (auto &button : levelButtons)
    {
        button.draw(window); // Draw each level button
    }
    returnButton.draw(window); // Draw the "Return" button
}