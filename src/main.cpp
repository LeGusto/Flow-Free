#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "FlowGrid.hpp"
#include "MainMenu.hpp"
#include "LevelReader.hpp"

enum class GameState
{
    MAIN_MENU,
    PLAYING
};

int main()
{
    GameState gameState = GameState::MAIN_MENU;
    auto window = sf::RenderWindow(sf::VideoMode({1000u, 800u}), "Flow Free", sf::Style::Close);

    window.setVerticalSyncEnabled(true);

    FlowGrid grid = readLevel(2, window);
    MainMenu mainMenu = MainMenu(window);

    std::string response = "";

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
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
                    gameState = GameState::PLAYING;
                }
            }
            else if (gameState == GameState::PLAYING)
            {
                grid.processEvent(event, window);
            }
        }

        window.clear();

        // grid.draw(window);
        // undoButton.draw(window);
        // redoButton.draw(window);
        if (gameState == GameState::MAIN_MENU)
        {
            mainMenu.draw(window);
            // mainMenu.handleInput(sf::Mouse::getPosition(window));
        }
        else if (gameState == GameState::PLAYING)
        {
            grid.draw(window);
        }

        window.display();
    }

    return 0;
}
