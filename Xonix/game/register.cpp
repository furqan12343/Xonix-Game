#include <SFML/Graphics.hpp>
#include"register.h"
#include "login.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace sf;
using namespace std;

bool usernameExists(const string& username,const string& password) 
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
            fileUsername=line.substr(userPos, line.find(" |",userPos)-userPos);

            passPos += 10; 
            filePassword = line.substr(passPos);

            // trim trailing spaces from passworrrd
            size_t pipePos=filePassword.find(" |");
            if (pipePos!=string::npos)
                filePassword=filePassword.substr(0, pipePos);

            if (fileUsername==username && filePassword==password)
                return true;
        }
    }
    return false;
}

bool isUpper(char ch) 
{
    return (ch>='A' && ch<='Z');
}

bool isLower(char ch) 
{
    return (ch>='a' && ch<='z');
}

bool isDigit(char ch) 
{
    return (ch>='0' && ch<='9');
}

bool isSpecial(char ch) 
{
    string specials="!@#$%^&*()";
    return (specials.find(ch)!=string::npos);
}

bool isStrongPassword(const string& password) 
{
    bool hasUpper=false,hasLower=false,hasDigit=false,hasSpecial=false;
    for (char ch: password) 
    {
        if (isUpper(ch))
        {
          hasUpper=true;
        }
        else if (isLower(ch)) 
        {
          hasLower=true;
        }
        else if (isDigit(ch))
        {
          hasDigit=true;
        }
        else if(isSpecial(ch)) 
        {
          hasSpecial=true;
        }
    }
    return password.length()>=8 && hasUpper && hasLower && hasDigit && hasSpecial;
}

string getTimestamp() 
{
    time_t now=time(0);
    tm* ltm=localtime(&now);
    stringstream ss;
    ss<<1900+ltm->tm_year<<"-"
      <<1+ltm->tm_mon<<"-"
      <<ltm->tm_mday<<" "
      <<1+ltm->tm_hour<<":"
      <<1+ltm->tm_min<<":"
      <<1+ltm->tm_sec;
    return ss.str();
}

int getNextID() 
{
    ifstream file("users.txt");
    string line;
    int lastID=1000;
    while (getline(file, line)) 
    {
        if (line.find("ID: ")!=string::npos) 
        {
            int id=stoi(line.substr(4, line.find(" ") - 4));
            lastID=max(lastID, id);
        }
    }
    return lastID+1;
}

bool registerpage() 
{
    RenderWindow window(VideoMode(800, 600),"Register Page");
    Font font;
    if (!font.loadFromFile("arial.ttf")) 
    {
        cout<<"Font loading failed!"<<endl;
        return -1;
    }

    RectangleShape background(Vector2f(800,600));
    background.setFillColor(Color(30,30,40));

    RectangleShape card(Vector2f(400,500));
    card.setFillColor(Color(45,45,60));
    card.setOutlineColor(Color(80,80,120));
    card.setOutlineThickness(1);
    card.setPosition(200,50);

    RectangleShape inputBoxes[3];
    for(int i=0;i<3;i++) 
    {
        inputBoxes[i].setSize(Vector2f(300,45));
        inputBoxes[i].setPosition(250,140+i*70);
        inputBoxes[i].setFillColor(Color(60,60,80));
        inputBoxes[i].setOutlineColor(Color(110,110,150));
        inputBoxes[i].setOutlineThickness(1);
    }

    Text labels[3];
    string labelNames[3]={"Username","Password","Confirm Password"};
    for(int i=0;i<3;i++) 
    {
        labels[i].setFont(font);
        labels[i].setString(labelNames[i]);
        labels[i].setCharacterSize(18);
        labels[i].setFillColor(Color(180,180,200));
        labels[i].setPosition(250,inputBoxes[i].getPosition().y - 25);
    }


    string inputs[3]={"", "", ""};
    Text inputText[3];
    for (int i=0;i<3;i++) 
    {
        inputText[i].setFont(font);
        inputText[i].setCharacterSize(20);
        inputText[i].setFillColor(Color(240, 240, 240));
        inputText[i].setPosition(inputBoxes[i].getPosition().x + 5, inputBoxes[i].getPosition().y + 10);
    }

    Text heading("Create Your Account", font, 26);
    heading.setFillColor(Color::White);
    heading.setPosition(250,70);

    RectangleShape signupBtn(Vector2f(300,45));
    signupBtn.setFillColor(Color(40,140,200));
    signupBtn.setPosition(250,370);
    Text signupText("Sign Up",font,20);
    signupText.setFillColor(Color::White);
    signupText.setPosition(355,377);

    RectangleShape backBtn(Vector2f(300,40));
    backBtn.setFillColor(Color(100,100,120));
    backBtn.setPosition(250,430);
    Text backText("Back",font,18);
    backText.setFillColor(Color::White);
    backText.setPosition(370,437);

    int focused=-1;
    Clock cursorClock;
    bool showCursor=true;
    string errorMsg="";
    Text errorText("",font,16);
    errorText.setFillColor(Color(255,80,80));
    errorText.setPosition(250,485);

    bool showPassword=false;
    bool showConfirmPassword=false;

    Texture eyeTexture;
    if (!eyeTexture.loadFromFile("eye_icon.png")) 
    {
        cout<<"Failed to load eye icon!"<<endl;
        return -1;
    }
    Sprite eyeIcon(eyeTexture);
    eyeIcon.setPosition(510,210);
    Sprite eyeIconConfirm(eyeTexture);
    eyeIconConfirm.setPosition(510,280);

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
                for (int i=0;i<3;i++) 
                {
                    if (inputBoxes[i].getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y)) 
                    {
                        focused=i;
                        break;
                    }
                }

                if (signupBtn.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y)) 
                {
                    errorMsg="";

                    if (inputs[0].empty() || inputs[1].empty() || inputs[2].empty()) 
                    {
                        errorMsg="All fields are required.";
                    } 
                    else if(usernameExists(inputs[0],inputs[1])) 
                    {
                        errorMsg="Username or password already exists.";
                    }
                    else if(!isStrongPassword(inputs[1])) 
                    {
                        errorMsg="Weak password: Use upper, lower, digit, special, 8+ chars.";
                    }
                    else if (inputs[1]!=inputs[2]) 
                    {
                        errorMsg="Passwords do not match.";
                    } 
                    else 
                    {
                        int id=getNextID();
                        string timestamp=getTimestamp();
                        ofstream file("users.txt",ios::app);
                        file<<"ID: "<<id
                            <<" | Username: "<<inputs[0]
                            <<" | Password: "<<inputs[1]
                            <<" | Timestamp: "<<timestamp<<endl;
                        file.close();
                        errorMsg="Registration Succesfull";
                        window.close();
                        loginpage();
                        return true;
                    }
                }

                if (backBtn.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y)) 
                {
                    window.close();
                    loginpage();
                }

                if (eyeIcon.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y)) 
                {
                    showPassword=!showPassword;
                }

                if (eyeIconConfirm.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y)) 
                {
                    showConfirmPassword=!showConfirmPassword;
                }
            }

            if (event.type==Event::TextEntered && focused!=-1) 
            {
                if (event.text.unicode==8) 
                {
                    if (!inputs[focused].empty())
                    {
                      inputs[focused].pop_back();
                    }  
                }
                else if (event.text.unicode >= 32 && event.text.unicode <= 126) 
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

        for (int i = 0; i < 3; ++i) 
        {
            string disp=(i==1 || i==2) ? (i==1 ? (showPassword ? inputs[i] : string(inputs[i].size(),'*')) :
                (showConfirmPassword ? inputs[i] : string(inputs[i].size(), '*'))) : inputs[i];
            if (i == focused && showCursor) 
            {
              disp += "|";
            }
            inputText[i].setString(disp);
        }

        Vector2i mousePos=Mouse::getPosition(window);
        signupBtn.setFillColor(signupBtn.getGlobalBounds().contains(mousePos.x, mousePos.y) ? Color(60,160,220) : Color(40,140,200));
        backBtn.setFillColor(backBtn.getGlobalBounds().contains(mousePos.x, mousePos.y) ? Color(120,120,140) : Color(100,100,120));
        errorText.setString(errorMsg);

        window.clear();
        window.draw(background);
        window.draw(card);
        window.draw(heading);
        for (int i=0;i<3;i++) 
        {
            window.draw(inputBoxes[i]);
            window.draw(labels[i]);
            window.draw(inputText[i]);
        }
        window.draw(signupBtn); window.draw(signupText);
        window.draw(backBtn); window.draw(backText);
        window.draw(errorText);
        window.draw(eyeIcon);
        window.draw(eyeIconConfirm);
        window.display();
    }

    return false;
}

