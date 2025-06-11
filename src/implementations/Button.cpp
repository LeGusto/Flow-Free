#include "../headers/Button.hpp"

Button::Button()
{
    if (!font.openFromFile(Defaults::HOME_DIR + "/src/ARIAL.TTF"))
    {
        std::cerr << "Error loading font" << std::endl;
    }
    text.setFont(font);
}

Button::Button(const Button &other)
{
    font = other.font;
    text = other.text;
    shape = other.shape;
}

Button::~Button() = default;

void Button::setText(const std::string &label)
{
    text.setFont(font);
    text.setString(label);
    text.setPosition(sf::Vector2f(
        shape.getPosition().x + (shape.getSize().x - text.getLocalBounds().size.x) / 2,
        shape.getPosition().y + (shape.getSize().y - text.getLocalBounds().size.y) / 2 - text.getCharacterSize() / 3));
}

void Button::setSize(sf::Vector2f size)
{
    shape.setSize(size);
}

void Button::setPosition(sf::Vector2f position)
{
    shape.setPosition(position);
    text.setPosition(sf::Vector2f(
        position.x + (shape.getSize().x - text.getLocalBounds().size.x) / 2,
        position.y + (shape.getSize().y - text.getLocalBounds().size.y) / 2 - text.getCharacterSize() / 3));
}

void Button::setColor(sf::Color color)
{
    shape.setFillColor(color);
}

void Button::setOutlineColor(sf::Color color)
{
    shape.setOutlineColor(color);
}

void Button::setOutlineThickness(float thickness)
{
    shape.setOutlineThickness(thickness);
}

void Button::setTextColor(sf::Color color)
{
    text.setFillColor(color);
}

void Button::setTextSize(unsigned short size)
{
    text.setCharacterSize(size);
}

sf::Vector2f Button::getSize()
{
    return shape.getSize();
}

void Button::setTextRotation(sf::Angle angle)
{
    text.setRotation(angle);
}

void Button::draw(sf::RenderWindow &window)
{
    window.draw(shape);
    text.setFont(font);
    window.draw(text);
}

bool Button::isClicked(const sf::Vector2i &mousePos)
{
    return shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

Button makeReturnButton()
{
    Button button = Button();
    button.setSize(sf::Vector2f(100, 50));
    button.setColor(sf::Color::Black);
    button.setText("Return");
    button.setTextColor(sf::Color::Red);
    button.setOutlineColor(sf::Color::Red);
    button.setOutlineThickness(Defaults::BUTTON_OUTLINE_THICKNESS);
    return button;
}

Button makeLevelButton()
{
    Button button = Button();
    button.setTextSize(20);
    button.setSize(sf::Vector2f(100, 50));
    button.setColor(sf::Color::Black);
    button.setOutlineColor(sf::Color::White);
    button.setOutlineThickness(Defaults::BUTTON_OUTLINE_THICKNESS);
    button.setTextColor(sf::Color::White);
    return button;
}

Button makeUndoButton()
{
    Button button = Button();
    button.setSize(sf::Vector2f(50, 50));
    button.setColor(sf::Color::Black);
    button.setOutlineColor(sf::Color::White);
    button.setTextSize(40);
    button.setText("<");
    button.setOutlineThickness(Defaults::BUTTON_OUTLINE_THICKNESS);
    return button;
}

Button makeRedoButton()
{
    Button button = Button();
    button.setSize(sf::Vector2f(50, 50));
    button.setColor(sf::Color::Black);
    button.setOutlineColor(sf::Color::White);
    button.setTextSize(40);
    button.setText(">");
    button.setOutlineThickness(Defaults::BUTTON_OUTLINE_THICKNESS);
    return button;
}

Button makeStartButton()
{
    Button button = Button();
    button.setSize(Defaults::MENU_BUTTON_SIZE);
    button.setColor(sf::Color::Black);
    button.setOutlineColor(sf::Color::White);
    button.setText("Start");
    button.setOutlineThickness(Defaults::BUTTON_OUTLINE_THICKNESS);
    return button;
}

Button makeExitButton()
{
    Button button = Button();
    button.setSize(Defaults::MENU_BUTTON_SIZE);
    button.setColor(sf::Color::Black);
    button.setOutlineColor(sf::Color::Red);
    button.setText("Exit");
    button.setTextColor(sf::Color::Red);
    button.setOutlineThickness(Defaults::BUTTON_OUTLINE_THICKNESS);
    return button;
}