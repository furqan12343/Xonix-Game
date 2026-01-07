#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "login.h"
#include "register.h"
using namespace sf;
using namespace std;

bool isValidUser(const string& username, const string& password) 
{
    ifstream file("users.txt");
    string line;

    while (getline(file, line)) 
    {
        string fileUsername, filePassword;

        size_t userPos=line.find("Username: ");
        size_t passPos=line.find("Password: ");

        if (userPos!=string::npos && passPos!=string::npos) 
        {
            userPos+=10;
            fileUsername=line.substr(userPos, line.find(" |", userPos) - userPos);

            passPos+=10;
            filePassword=line.substr(passPos);

            size_t pipePos=filePassword.find(" |");
            if (pipePos!=string::npos)
                filePassword=filePassword.substr(0, pipePos);

            if (fileUsername==username && filePassword==password)
                return true;
        }
    }

    return false;
}

bool loginpage() 
{
    RenderWindow window(VideoMode(800, 600), "Login Page");
    Font font;
    if (!font.loadFromFile("arial.ttf")) 
    {
        cout<<"Font loading failed!"<<endl;
        return -1;
    }

    RectangleShape background(Vector2f(800,600));
    background.setFillColor(Color(30,30,40));
    // Create the main card
    RectangleShape card(Vector2f(400,500));
    card.setFillColor(Color(45,45,60));
    card.setOutlineColor(Color(80,80,120));
    card.setOutlineThickness(1);
    card.setPosition(200,50);

    // Create input boxes
    RectangleShape inputBoxes[2];
    for (int i=0;i<2;i++) 
    {
        inputBoxes[i].setSize(Vector2f(300,45));
        inputBoxes[i].setPosition(250,140+i*70);
        inputBoxes[i].setFillColor(Color(60,60,80));
        inputBoxes[i].setOutlineColor(Color(110,110,150));
        inputBoxes[i].setOutlineThickness(1);
    }

    // Create labels for inputs
    Text labels[2];
    string labelNames[2]={"Username", "Password"};
    for (int i=0;i<2;i++) 
    {
        labels[i].setFont(font);
        labels[i].setString(labelNames[i]);
        labels[i].setCharacterSize(18);
        labels[i].setFillColor(Color(180,180,200));
        labels[i].setPosition(250,inputBoxes[i].getPosition().y-25);
    }

    // Store input text
    string inputs[2] = {"", ""};
    Text inputText[2];
    for (int i=0;i<2;i++) 
    {
        inputText[i].setFont(font);
        inputText[i].setCharacterSize(20);
        inputText[i].setFillColor(Color(240,240,240));
        inputText[i].setPosition(inputBoxes[i].getPosition().x + 5, inputBoxes[i].getPosition().y + 10);
    }

    // Heading text
    Text heading("Login to Your Account",font,26);
    heading.setFillColor(Color::White);
    heading.setPosition(250,70);

    // Create login button
    RectangleShape loginBtn(Vector2f(300,45));
    loginBtn.setFillColor(Color(40,140,200));
    loginBtn.setPosition(250,300);
    Text loginText("Login",font,20);
    loginText.setFillColor(Color::White);
    loginText.setPosition(374,309);

    // Create register button
    RectangleShape registerBtn(Vector2f(300,40));
    registerBtn.setFillColor(Color(230,230,230));
    registerBtn.setPosition(250,360);
    Text registerText("Register",font,18);
    registerText.setFillColor(Color::Black);
    registerText.setPosition(374,370);
    
    // Create back button
    RectangleShape backBtn(Vector2f(300,40));
    backBtn.setFillColor(Color(100,100,120));
    backBtn.setPosition(250,420);
    Text backText("Back",font,18);
    backText.setFillColor(Color::Black);
    backText.setPosition(360,430);


    // Error message text
    string errorMsg="";
    Text errorText("", font, 16);
    errorText.setFillColor(Color(255,80,80));
    errorText.setPosition(250,500);

    // Eye icon for password visibility toggle
    Texture eyeTexture;
    if (!eyeTexture.loadFromFile("eye_icon.png")) 
    {
        cout<<"Failed to load eye icon!"<<endl;
        return -1;
    }
    Sprite eyeIcon(eyeTexture);
    eyeIcon.setPosition(510, 210);
    bool showPassword=false;

    // Track the currently focused input field
    int focused=-1;
    Clock cursorClock;
    bool showCursor=true;

    while (window.isOpen()) 
    {
        Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type==Event::Closed)
                window.close();

            if (event.type==Event::MouseButtonPressed) 
            {
                focused=-1;
                for (int i=0;i<2;i++) 
                {
                    if (inputBoxes[i].getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y)) 
                    {
                        focused=i;
                        break;
                    }
                }

                if (loginBtn.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y)) 
                {
                    errorMsg = "";
                    if (inputs[0].empty() || inputs[1].empty()) 
                    {
                        errorMsg="Please enter both fields.";
                    } 
                    else if (!isValidUser(inputs[0], inputs[1])) 
                    {
                        errorMsg="Invalid username or password.";
                    } 
                    else 
                    {
                        ofstream activeFile("activeuser.txt");
                        if (activeFile.is_open()) 
                        {
                            activeFile<<inputs[0]<<" "<<0<<"\n";
                            activeFile.close();
                        } 
                        else 
                        {
                            cerr<<"Could not open activeuser.txt for writing."<<endl;
                        }
                        window.close();
                        return true;
                    }
                }

                if (backBtn.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y)) 
                {
                    cout<<"Back clicked."<<endl;
                    window.close();
                }

                if (registerBtn.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y)) 
                {
                    cout<<"Register clicked."<<endl;
                    window.close();
                    registerpage();
                }

                if (eyeIcon.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y)) 
                {
                    showPassword=!showPassword;
                }
            }

            if (event.type==Event::TextEntered && focused!=-1) 
            {
                if (event.text.unicode==8) 
                {
                    if(!inputs[focused].empty())
                    {
                      inputs[focused].pop_back();
                    }
                } 
                else if(event.text.unicode>=32 && event.text.unicode<=126) 
                {
                    inputs[focused] += static_cast<char>(event.text.unicode);
                }
            }
        }

        if (cursorClock.getElapsedTime().asSeconds()>0.5f) 
        {
            showCursor=!showCursor;
            cursorClock.restart();
        }

        for (int i=0;i<2;i++) 
        {
            string disp=(i==1)?(showPassword? inputs[i] : string(inputs[i].size(), '*')) : inputs[i];
            if (i == focused && showCursor) 
            {
              disp+="|";
            } 
            inputText[i].setString(disp);
        }

        Vector2i mousePos=Mouse::getPosition(window);


        loginBtn.setFillColor(loginBtn.getGlobalBounds().contains(mousePos.x, mousePos.y) ? Color(65,105,225) : Color(100,149,237));
        backBtn.setFillColor(backBtn.getGlobalBounds().contains(mousePos.x, mousePos.y) ? Color(200,200,200) : Color(230,230,230));
        registerBtn.setFillColor(registerBtn.getGlobalBounds().contains(mousePos.x, mousePos.y) ? Color(200,200,200) : Color(230,230,230));

        if (eyeIcon.getGlobalBounds().contains(mousePos.x, mousePos.y)) 
        {
            eyeIcon.setColor(Color(200, 200, 255));
        } 
        else 
        {
            eyeIcon.setColor(Color::White);
        }

        errorText.setString(errorMsg);

        window.draw(background); 
        window.draw(card);
        window.draw(heading);
        for (int i = 0; i < 2; ++i) 
        {
            window.draw(inputBoxes[i]);
            window.draw(labels[i]);
            window.draw(inputText[i]);
        }
        window.draw(loginBtn); window.draw(loginText);
        window.draw(backBtn); window.draw(backText);
        window.draw(registerBtn); window.draw(registerText);
        window.draw(errorText);
        window.draw(eyeIcon);
        window.display();
    }

    return false;
}

