#ifndef GAME_H
#define GAME_H

#include<string>
#include <SFML/Graphics.hpp>
#include"gamestructure.h"
using namespace sf;



class Background
{
public:
    Background(Color bgColor) 
    {
        // Set the background color when the object is created
        backgroundColor = bgColor;
    }

    void draw(RenderWindow& window) 
    {
        // Create a rectangle to fill the background
        RectangleShape background(Vector2f(window.getSize()));
        background.setFillColor(backgroundColor);
        window.draw(background);
    }

private:
    Color backgroundColor;
};

int runXonixGame(int enemyCount, int R, int G, int B, GameStructure* resume);
int multiplayer(int id1,int id2);

#endif

