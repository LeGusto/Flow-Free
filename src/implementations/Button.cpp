#include "../headers/Button.hpp"

// Initializes the button and loads the font
Button::Button()
{
    if (!font.openFromFile(Defaults::HOME_DIR + "/src/ARIAL.TTF"))
    {
        std::cerr << "Error loading font" << std::endl;
    }
    text.setFont(font);
}

// Copy constructor for Button
Button::Button(const Button &other)
{
    font = other.font;
    text = other.text;
    shape = other.shape;
}

Button::~Button() = default;

// Sets the text displayed on the button
void Button::setText(const std::string &label)
{
    text.setFont(font);
    text.setString(label);
    text.setPosition(sf::Vector2f(
        shape.getPosition().x + (shape.getSize().x - text.getLocalBounds().size.x) / 2,
        shape.getPosition().y + (shape.getSize().y - text.getLocalBounds().size.y) / 2 - text.getCharacterSize() / 3)); // Center the text
}

// Sets the size of the button
void Button::setSize(sf::Vector2f size)
{
    shape.setSize(size);
}

// Sets the position of the button
void Button::setPosition(sf::Vector2f position)
{
    shape.setPosition(position);
    text.setPosition(sf::Vector2f(
        position.x + (shape.getSize().x - text.getLocalBounds().size.x) / 2,
        position.y + (shape.getSize().y - text.getLocalBounds().size.y) / 2 - text.getCharacterSize() / 3)); // Center the text
}

// Sets the fill color of the button
void Button::setColor(sf::Color color)
{
    shape.setFillColor(color);
}

// Sets the outline color of the button
void Button::setOutlineColor(sf::Color color)
{
    shape.setOutlineColor(color);
}

// Sets the outline thickness of the button
void Button::setOutlineThickness(float thickness)
{
    shape.setOutlineThickness(thickness);
}

// Sets the color of the button text
void Button::setTextColor(sf::Color color)
{
    text.setFillColor(color);
}

// Sets the size of the button text
void Button::setTextSize(unsigned short size)
{
    text.setCharacterSize(size);
}

// Returns the size of the button
sf::Vector2f Button::getSize()
{
    return shape.getSize();
}

// Sets the rotation angle of the button text
void Button::setTextRotation(sf::Angle angle)
{
    text.setRotation(angle);
}

// Draws the button on the SFML render window
void Button::draw(sf::RenderWindow &window)
{
    window.draw(shape);
    text.setFont(font);
    window.draw(text);
}

// Checks if the button was clicked based on the mouse position
bool Button::isClicked(const sf::Vector2i &mousePos)
{
    return shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)); // Check if the mouse position is within the button bounds
}

// Factory function to create a "Return" button
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

// Factory function to create a level button
Button makeLevelButton()
{
    Button button = Button();
    button.setTextSize(20);                                         // Set the text size
    button.setSize(sf::Vector2f(100, 50));                          // Set the size of the button
    button.setColor(sf::Color::Black);                              // Set the fill color
    button.setOutlineColor(sf::Color::White);                       // Set the outline color
    button.setOutlineThickness(Defaults::BUTTON_OUTLINE_THICKNESS); // Set the outline thickness
    button.setTextColor(sf::Color::White);                          // Set the text color
    return button;
}

// Factory function to create an "Undo" button
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

// Factory function to create a "Redo" button
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

// Factory function to create a "Start" button
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

// Factory function to create an "Exit" button
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