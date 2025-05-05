
#include <catch2/catch_test_macros.hpp>
#include "../headers/PathManager.hpp"
#include "../headers/Cells.hpp"

TEST_CASE("Path initialization", "[Path]")
{
    sf::Vector2f origin(0, 0);
    sf::Color color = sf::Color::Red;
    u_short remainingSources = 5;

    Path path(&origin, color, &remainingSources);

    REQUIRE(path.origin == &origin);
    REQUIRE(path.color == color);
    REQUIRE(path.remainingSources == &remainingSources);
}

TEST_CASE("PathMaker initialization", "[PathMaker]")
{
    sf::Vector2f origin(0, 0);
    u_short remainingSources = 5;

    PathMaker pathMaker(&origin, &remainingSources);

    REQUIRE(pathMaker.getPath() == nullptr);
    REQUIRE(pathMaker.isPathDrawing() == false);
}

TEST_CASE("Path extension and shrinking", "[Path]")
{
    sf::Vector2f origin(0, 0);
    sf::Color color = sf::Color::Blue;
    u_short remainingSources = 5;

    Path path(&origin, color, &remainingSources);

    Cell cell1(0, 0, sf::Vector2f(10, 10), 0, 0);
    Cell cell2(0, 1, sf::Vector2f(10, 10), 0, 0);

    path.extendPath(&cell1);
    REQUIRE(path.cells.size() == 1);
    REQUIRE(path.cells.back() == &cell1);

    path.extendPath(&cell2);
    REQUIRE(path.cells.size() == 2);
    REQUIRE(path.cells.back() == &cell2);

    path.shrinkPath();
    REQUIRE(path.cells.size() == 1);
    REQUIRE(path.cells.back() == &cell1);
}

TEST_CASE("Path clearing", "[Path]")
{
    sf::Vector2f origin(0, 0);
    sf::Color color = sf::Color::Green;
    u_short remainingSources = 5;

    Path path(&origin, color, &remainingSources);

    Cell cell1(0, 0, sf::Vector2f(10, 10), 0, 0);
    Cell cell2(0, 1, sf::Vector2f(10, 10), 0, 0);

    path.extendPath(&cell1);
    path.extendPath(&cell2);

    REQUIRE(path.cells.size() == 2);

    path.clearPath();
    REQUIRE(path.cells.empty());
    REQUIRE(path.line.empty());
}

TEST_CASE("Path validation 1", "[PathMaker]")
{
    sf::Vector2f origin(0, 0);
    u_short remainingSources = 5;

    SECTION("Test row and column differences between cells in path")
    {
        PathMaker pathMaker(&origin, &remainingSources);

        Cell cell1(0, 0, sf::Vector2f(10, 10), 0, 0);
        Cell cell2(0, 1, sf::Vector2f(10, 10), 0, 0);
        Cell cell3(0, 2, sf::Vector2f(10, 10), 0, 0);

        pathMaker.startPath(&cell1, sf::Color::Red);

        std::vector<Cell *> validPath = {&cell2, &cell3};
        std::vector<Cell *> invalidPath = {&cell3, &cell2};

        pathMaker.startPath(&cell1, sf::Color::Yellow);
        REQUIRE(pathMaker.validatePath(validPath) == true);
        REQUIRE(pathMaker.validatePath(invalidPath) == false);
    }

    SECTION("Test invalid and valid endpoint of path")
    {
        sf::Vector2f origin(0, 0);
        u_short remainingSources = 5;

        PathMaker pathMaker(&origin, &remainingSources);
        sf::CircleShape sf1(10);
        sf1.setFillColor(sf::Color::Yellow);

        Cell cell1(0, 0, sf::Vector2f(10, 10), 0, 0);
        Cell cell2(0, 1, sf::Vector2f(10, 10), 0, 0);
        SourceCell cell3(0, 2, sf::Vector2f(10, 10), 0, 0, sf1);

        std::vector<Cell *> tPath = {&cell2, &cell3};
        pathMaker.startPath(&cell1, sf::Color::Red);
        REQUIRE(pathMaker.validatePath(tPath) == false);

        pathMaker.startPath(&cell1, sf::Color::Yellow);
        REQUIRE(pathMaker.validatePath(tPath) == true);
    }
}

TEST_CASE("Path finalization", "[Path]")
{
    sf::Vector2f origin(0, 0);
    sf::Color color = sf::Color::Cyan;
    u_short remainingSources = 5;

    Path path(&origin, color, &remainingSources);

    Cell cell1(0, 0, sf::Vector2f(10, 10), 0, 0);
    Cell cell2(0, 1, sf::Vector2f(10, 10), 0, 0);

    path.extendPath(&cell1);
    path.extendPath(&cell2);

    path.finalizePath();
    REQUIRE(path.completed == true);
    REQUIRE(remainingSources == 4);
}

TEST_CASE("PathMaker path drawing", "[PathMaker]")
{
    sf::Vector2f origin(0, 0);
    u_short remainingSources = 5;

    PathMaker pathMaker(&origin, &remainingSources);

    sf::CircleShape sf1(10);
    sf1.setFillColor(sf::Color::Yellow);
    sf::CircleShape sf2(10);
    sf2.setFillColor(sf::Color::Yellow);

    SourceCell cell1(0, 0, sf::Vector2f(10, 10), 0, 0, sf1);
    SourceCell cell2(0, 1, sf::Vector2f(10, 10), 0, 0, sf2);

    REQUIRE(remainingSources == 5);
    pathMaker.startPath(&cell1, sf::Color::Yellow);
    std::vector<Cell *> cells = {&cell2};
    REQUIRE(remainingSources == 5);
    pathMaker.addCells(cells);

    REQUIRE(remainingSources == 4);
    REQUIRE(pathMaker.isPathDrawing() == false);
}