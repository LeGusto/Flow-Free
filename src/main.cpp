#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "FlowGrid.hpp"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1000u, 800u}), "Flow Free", sf::Style::Close);

    window.setVerticalSyncEnabled(true);

    std::vector<ColorNodes> colorNodes = {
        {10, 0, 0, 1, sf::Color::Red},
        {1, 5, 1, 1, sf::Color::Blue},
        {2, 7, 2, 5, sf::Color::Green},
        {0, 4, 3, 1, sf::Color::Yellow},
        {9, 9, 4, 1, sf::Color::Magenta},
        {5, 0, 5, 1, sf::Color::Cyan}};

    FlowGrid grid = FlowGrid(10, 10, 50, colorNodes, window);
    Button undoButton = Button(-grid.getOrigin().x, -grid.getOrigin().y - 50, 100, 50, sf::Color::Red, "Undo");
    Button redoButton = Button(-grid.getOrigin().x + 100, -grid.getOrigin().y - 50, 100, 50, sf::Color::Green, "Redo");

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto *mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    // Checks if a cell is clicked
                    grid.makePath(mouseButtonPressed->position.x, mouseButtonPressed->position.y);
                    if (undoButton.isClicked(mouseButtonPressed->position))
                    {
                        grid.undo();
                    }
                    else if (redoButton.isClicked(mouseButtonPressed->position))
                    {
                        grid.redo();
                    }
                }
                else if (mouseButtonPressed->button == sf::Mouse::Button::Right)
                {
                    if (grid.isDrawing())
                    {
                        grid.destroyPath();
                    }
                }
            }

            if (const auto *mouseMoved = event->getIf<sf::Event::MouseMoved>())
            {
                if (grid.isDrawing())
                {
                    grid.makePath(mouseMoved->position.x, mouseMoved->position.y);
                }
            }
        }

        window.clear();

        grid.draw(window);
        undoButton.draw(window);
        redoButton.draw(window);

        window.display();
    }

    return 0;
}
