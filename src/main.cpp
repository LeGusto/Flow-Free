#include <SFML/Graphics.hpp>
#include "FlowGrid.hpp"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1000u, 800u}), "Flow Free", sf::Style::Close);
    // window.setFramerateLimit(144);
    window.setVerticalSyncEnabled(true);

    
    FlowGrid grid = FlowGrid(10, 10, 50);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();

        grid.draw(window);

        window.display();
    }
}
