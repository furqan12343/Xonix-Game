#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "menu.h"
#include "game.h"
#include<cmath>
using namespace std;

Menu::Menu(float width, float height)
{

       if (!hoverBuffer.loadFromFile("hover.wav"))
{
    cout << "Failed to load hover sound.";
}
if (!selectBuffer.loadFromFile("select.wav"))
{
    cout << "Failed to load select sound.";
}
hoverSound.setBuffer(hoverBuffer);
selectSound.setBuffer(selectBuffer);
    if (!font.loadFromFile("ByteBounce.ttf"))
    {
        cout<<"Error loading font";
    }

    // Title
    title.setFont(font);
    title.setString("Xonix");
    title.setFillColor(sf::Color::Yellow);
    title.setCharacterSize(100);
    title.setStyle(sf::Text::Bold);

    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top + titleBounds.height / 2.0f);
    title.setPosition(sf::Vector2f(width / 2.0f, height * 0.15f));

    string labels[MAX_NUMBER_OF_ITEMS] = { "Start Game", "Resume", "Select Level", "Leaderboard", "Themes", "Logout", "Back" };

    for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
    {
        menu[i].setFont(font);
        menu[i].setFillColor(sf::Color::White);
        menu[i].setString(labels[i]);
        menu[i].setCharacterSize(50);
        menu[i].setStyle(sf::Text::Bold);

        sf::FloatRect textBounds = menu[i].getLocalBounds();
        menu[i].setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);

        float posX = width / 2;
        float posY = height * 0.35f + i * (height * 0.1f);
        menu[i].setPosition(posX, posY);

        // Set button size and position
        buttons[i].setSize(sf::Vector2f(textBounds.width + 60, textBounds.height + 40));
        buttons[i].setOrigin(buttons[i].getSize() / 2.0f);
        buttons[i].setPosition(posX, posY);
        buttons[i].setOutlineThickness(4);
        buttons[i].setOutlineColor(sf::Color::Black);

        // Rounded corner using CircleShape
        for (int j = 0; j < 4; j++)
        {
            corners[j].setRadius(15);  // Corner radius
            corners[j].setPointCount(30);  // Smooth circle
            corners[j].setFillColor(sf::Color(70, 130, 180)); // Corner color
        }

        // Set positions for each corner (top-left, top-right, bottom-left, bottom-right)
        corners[0].setPosition(posX - buttons[i].getSize().x / 2, posY - buttons[i].getSize().y / 2);  // Top-left
        corners[1].setPosition(posX + buttons[i].getSize().x / 2 - 30, posY - buttons[i].getSize().y / 2); // Top-right
        corners[2].setPosition(posX - buttons[i].getSize().x / 2, posY + buttons[i].getSize().y / 2 - 30); // Bottom-left
        corners[3].setPosition(posX + buttons[i].getSize().x / 2 - 30, posY + buttons[i].getSize().y / 2 - 30); // Bottom-right
    }

    selectedItemIndex = 0;
    glowTime = 0.f;
}

void Menu::draw(sf::RenderWindow &window)
{
    window.draw(title);
    for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
    {
        // Draw button
        window.draw(buttons[i]);

        // Draw rounded corners
        for (int j = 0; j < 4; j++)
        {
            window.draw(corners[j]);
        }

        window.draw(menu[i]);
    }
}

void Menu::update(float deltaTime)
{
    glowTime += deltaTime * 5; // Speed of pulsing

    float glow = std::abs(std::sin(glowTime)) * 100 + 155; // Value between 155-255
    sf::Color glowColor(glow, 69, 0); // Orange-red pulsing

    for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
    {
        if (i == selectedItemIndex)
            buttons[i].setFillColor(glowColor);
        else
            buttons[i].setFillColor(sf::Color(70, 130, 180)); // Unselected
    }
}

void Menu::MoveUp()
{
    if (selectedItemIndex - 1 >= 0)
    {
        selectedItemIndex--;
        hoverSound.play();
    }
}

void Menu::MoveDown()
{
    if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
    {
        selectedItemIndex++;
        hoverSound.play();
    }
}

int Menu::GetPressedItem() const
{
    return selectedItemIndex;
}

void Menu::select()
{
    selectSound.play();
}

