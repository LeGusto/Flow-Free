#include <SFML/Graphics.hpp>
#include "../headers/Button.hpp"
#include "../headers/FlowGrid.hpp"
#include "../headers/MainMenu.hpp"
#include "../headers/LevelReader.hpp"
#include "../headers/LevelSelection.hpp"
#include "../headers/CompletedScreen.hpp"

enum class GameState
{
    MAIN_MENU,
    SELECT_LEVEL,
    PLAYING,
    COMPLETED_LEVEL
};

int main()
{
    // Initialize the game state to start at the main menu
    GameState gameState = GameState::MAIN_MENU;

    // Create the main game window
    auto window = sf::RenderWindow(sf::VideoMode({1000u, 800u}), "Flow Free", sf::Style::Close);
    window.setVerticalSyncEnabled(true); // Enable vertical sync for smoother rendering

    // Load the initial level and initialize UI components
    FlowGrid grid = readLevel(2, window); // Loads level 2 as default
    MainMenu mainMenu = MainMenu(window);
    LevelSelection levelSelection = LevelSelection(window);
    CompletedScreen completedScreen = CompletedScreen(window);

    std::string response = ""; // Variable to store user responses (e.g., button clicks)
    unsigned short currLevel = -1;

    // Main game loop
    while (window.isOpen())
    {
        // Event handling loop
        while (const std::optional event = window.pollEvent())
        {
            response = ""; // Reset the response for each event

            // Handle window close event
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            // Handle events based on the current game state
            if (gameState == GameState::MAIN_MENU)
            {
                // Handle clicks in the main menu
                mainMenu.handleClick(event, response);
                if (response == "Close")
                {
                    window.close(); // Close the window if "Close" is clicked
                }
                else if (response == "Start")
                {
                    gameState = GameState::SELECT_LEVEL; // Transition to the level selection screen
                }
            }
            else if (gameState == GameState::SELECT_LEVEL)
            {
                // Handle clicks in the level selection screen
                levelSelection.handleClick(event, response);
                if (response == "Return")
                {
                    gameState = GameState::MAIN_MENU; // Return to the main menu
                }
                else if (response != "")
                {
                    // Load the selected level
                    currLevel = std::stoi(response);                // Convert the response to an integer (level number)
                    grid = std::move(readLevel(currLevel, window)); // Load the selected level
                    gameState = GameState::PLAYING;                 // Transition to the gameplay screen
                }
            }
            else if (gameState == GameState::PLAYING)
            {
                // Handle gameplay events
                grid.processEvent(event, window, response);
                if (response == "Return")
                {
                    gameState = GameState::SELECT_LEVEL; // Return to the level selection screen
                }
                else if (grid.isCompleted())
                {
                    // Mark the level as completed and transition to the completion screen
                    levelSelection.setLevelCompleted(currLevel);
                    gameState = GameState::COMPLETED_LEVEL;
                }
            }
            else if (gameState == GameState::COMPLETED_LEVEL)
            {
                // Handle clicks in the level completion screen
                completedScreen.handleClick(event, response);
                if (response == "Return")
                {
                    gameState = GameState::SELECT_LEVEL; // Return to the level selection screen
                }
            }
        }

        // Clear the window for the next frame
        window.clear();

        // Draw the appropriate screen based on the current game state
        if (gameState == GameState::MAIN_MENU)
        {
            mainMenu.draw(window); // Draw the main menu
        }
        else if (gameState == GameState::SELECT_LEVEL)
        {
            levelSelection.draw(window); // Draw the level selection screen
        }
        else if (gameState == GameState::PLAYING)
        {
            grid.draw(window); // Draw the gameplay grid
        }
        else if (gameState == GameState::COMPLETED_LEVEL)
        {
            grid.draw(window);            // Draw the gameplay grid
            completedScreen.draw(window); // Draw the level completion screen
        }

        // Display the rendered frame
        window.display();
    }

    return 0;
}
