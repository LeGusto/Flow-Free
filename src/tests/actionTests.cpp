#include <catch2/catch_test_macros.hpp>
#include "../headers/ActionManager.hpp"
#include "../headers/PathManager.hpp"

TEST_CASE("ActionManager: Undo and Redo functionality", "[ActionManager]")
{
    sf::Vector2f origin(0, 0);
    unsigned short remainingSources = 1;
    Path path(&origin, sf::Color::Red, &remainingSources);
    Cell cell1(0, 0, sf::Vector2f(10, 10), 0, 0);
    path.extendPath(&cell1);
    ActionManager actionManager;

    REQUIRE_FALSE(path.cells.empty());

    SECTION("Undo ADD action")
    {
        Action addAction(Action::ADD, &path);
        actionManager.undoAddAction(addAction);

        actionManager.undo();

        REQUIRE(path.cells.empty());
    }

    SECTION("Redo ADD action")
    {
        Action addAction(Action::ADD, &path);
        actionManager.undoAddAction(addAction);

        actionManager.redo();

        REQUIRE_FALSE(path.cells.empty());
    }

    SECTION("Undo and Redo sequence")
    {
        Action addAction(Action::ADD, &path);
        actionManager.undoAddAction(addAction);

        actionManager.undo();

        actionManager.redo();

        actionManager.undo();

        REQUIRE(path.cells.empty());
    }
}