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
    GameState gameState = GameState::MAIN_MENU;
    auto window = sf::RenderWindow(sf::VideoMode({1000u, 800u}), "Flow Free", sf::Style::Close);

    window.setVerticalSyncEnabled(true);

    FlowGrid grid = readLevel(2, window);
    MainMenu mainMenu = MainMenu(window);
    LevelSelection levelSelection = LevelSelection(window);
    CompletedScreen completedScreen = CompletedScreen(window);

    std::string response = "";
    unsigned short currLevel = -1;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            response = "";
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (gameState == GameState::MAIN_MENU)
            {
                mainMenu.handleClick(event, response);
                if (response == "Close")
                {
                    window.close();
                }
                else if (response == "Start")
                {
                    gameState = GameState::SELECT_LEVEL;
                }
            }
            else if (gameState == GameState::SELECT_LEVEL)
            {
                levelSelection.handleClick(event, response);
                if (response == "Return")
                {
                    gameState = GameState::MAIN_MENU;
                }
                else if (response != "")
                {
                    currLevel = std::stoi(response);
                    grid = std::move(readLevel(currLevel, window));
                    gameState = GameState::PLAYING;
                }
            }
            else if (gameState == GameState::PLAYING)
            {
                grid.processEvent(event, window, response);
                if (response == "Return")
                {
                    gameState = GameState::SELECT_LEVEL;
                }
                else if (grid.isCompleted())
                {
                    levelSelection.setLevelCompleted(currLevel);
                    gameState = GameState::COMPLETED_LEVEL;
                }
            }
            else if (gameState == GameState::COMPLETED_LEVEL)
            {
                completedScreen.handleClick(event, response);
                if (response == "Return")
                {
                    gameState = GameState::SELECT_LEVEL;
                }
            }
        }

        window.clear();

        if (gameState == GameState::MAIN_MENU)
        {
            mainMenu.draw(window);
        }
        else if (gameState == GameState::SELECT_LEVEL)
        {
            levelSelection.draw(window);
        }
        else if (gameState == GameState::PLAYING)
        {
            grid.draw(window);
        }
        else if (gameState == GameState::COMPLETED_LEVEL)
        {
            grid.draw(window);
            completedScreen.draw(window);
        }

        window.display();
    }

    return 0;
}
