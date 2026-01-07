#ifndef MENU_H
#define MENU_H

#include<SFML/Graphics.hpp>
#include<string>

#define MAX_NUMBER_OF_ITEMS 7

class Menu 
{
private:
    int selectedItemIndex;
    sf::Font font;
    sf::Text menu[MAX_NUMBER_OF_ITEMS];
    sf::RectangleShape buttons[MAX_NUMBER_OF_ITEMS];
    sf::CircleShape corners[4];  // Declare the corners for rounded buttons
    sf::Text title; 
    sf::SoundBuffer hoverBuffer;
    sf::SoundBuffer selectBuffer;
    sf::Sound hoverSound;
    sf::Sound selectSound;

    float glowTime; // Declare glowTime here

public:
    Menu(float width, float height);
    void draw(sf::RenderWindow &window);
    void update(float deltaTime);  // Declare the update method here
    void MoveUp();
    void MoveDown();
    int GetPressedItem() const;
    void select();
};

#endif

