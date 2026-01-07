#include<SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<iostream>
#include"menu.h"
#include"game.h"
#include"login.h"
#include"register.h"
#include"leaderboard.h"
#include<fstream> 
#include"matchmaking.h"
#include"theme.h"
#include"gamestructure.h"
#include"friend_system.h"
#include<cmath>


#define NUM_LEVELS 3
int R=0,G=0,B=0;

void changebackground(sf::RenderWindow& window, sf::Texture& background_t,int r,int g,int b) 
{
    sf::Color backgroundColor(r,g,b);

    if (backgroundColor == sf::Color::Transparent) 
    {
        sf::Sprite backgroundSprite(background_t);
        window.clear(); 
        window.draw(backgroundSprite);
    } 
    else 
    {
        window.clear(backgroundColor); 
    }
}

class ThemeMenu
{
public:
sf::Texture background_t;
Theme themes[10];

ThemeMenu(float width,float height,ThemeInventory& inventory) 
{
    if (!font.loadFromFile("ByteBounce.ttf")) 
    {
        cout<<"font load failed"<<endl;
    }

    count=inventory.getThemes(themes);  // extract from AVL in theme.cpp

    float verticalSpacing=60.0f;
    float startY=height/2-(count*verticalSpacing)/2;

    for (int i=0;i<count;i++) 
    {
        themeText[i].setFont(font);
        themeText[i].setString(themes[i].getName());
        themeText[i].setCharacterSize(50);
        themeText[i].setFillColor(i==0?sf::Color::Red : sf::Color::White);

        sf::FloatRect bounds=themeText[i].getLocalBounds();
        themeText[i].setOrigin(bounds.left+bounds.width/2,bounds.top+bounds.height/2);
        themeText[i].setPosition(width/2,startY+i*verticalSpacing);
    }

    title.setFont(font);
    title.setString("Choose Theme");
    title.setCharacterSize(80);
    title.setFillColor(sf::Color::Cyan);

    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin(titleBounds.left+titleBounds.width/2,titleBounds.top+titleBounds.height/2);
    title.setPosition(width/2,height/6);

    theme_index=0;
}

void MoveUp() 
{
    if(theme_index>0) 
    {
        themeText[theme_index].setFillColor(sf::Color::White);
        --theme_index;
        themeText[theme_index].setFillColor(sf::Color::Red);
    }
}

void MoveDown() 
{
    if(theme_index<count-1) 
    {
        themeText[theme_index].setFillColor(sf::Color::White);
        ++theme_index;
        themeText[theme_index].setFillColor(sf::Color::Red);
    }
}

void draw(sf::RenderWindow& window) 
{
    window.draw(title);
    for (int i=0;i<count;i++) 
    {
        window.draw(themeText[i]);
    }
}

int get_themeid() const 
{
    return themes[theme_index].getId();
}

void setbackground_rgb(int r,int g,int b) 
{
    R=r;
    G=g;
    B=b;
}

void applyBackground(sf::RenderWindow& window) 
{
    changebackground(window,background_t,R,G,B);
}

private:
sf::Font font;
sf::Text themeText[10];
int count=0;
int theme_index;
sf::Text title;

};

class LevelMenu
{
public:
    LevelMenu(float width, float height)
    {
       if (!hoverBuffer.loadFromFile("hover.wav"))
{
    cout << "Failed to load hover sound.\n";
}
if (!selectBuffer.loadFromFile("select.wav"))
{
    cout << "Failed to load select sound.\n";
}
hoverSound.setBuffer(hoverBuffer);
selectSound.setBuffer(selectBuffer);

       if (!font.loadFromFile("ByteBounce.ttf"))
        {
            cout << "Error loading font.\n";
        }

        string levels[NUM_LEVELS] = { "Easy", "Medium", "Hard" };

        for (int i = 0; i < NUM_LEVELS; i++)
        {
            levelText[i].setFont(font);
            levelText[i].setString(levels[i]);
            levelText[i].setCharacterSize(50);
            levelText[i].setStyle(sf::Text::Bold);
            levelText[i].setFillColor(sf::Color::White);

            sf::FloatRect textBounds = levelText[i].getLocalBounds();
            levelText[i].setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);

            float posX = width / 2;
            float posY = height * 0.3f + i * (height * 0.15f);
            levelText[i].setPosition(posX, posY);

            buttons[i].setSize(sf::Vector2f(textBounds.width + 60, textBounds.height + 40));
            buttons[i].setOrigin(buttons[i].getSize() / 2.0f);
            buttons[i].setPosition(posX, posY);
            buttons[i].setOutlineThickness(4);
            buttons[i].setOutlineColor(sf::Color::Black);
        }

        level_index = 0;
        glowTime = 0.f;
    }

    void draw(sf::RenderWindow& window)
    {
        for (int i = 0; i < NUM_LEVELS; i++)
        {
            window.draw(buttons[i]);
            window.draw(levelText[i]);
        }
    }

    void update(float dt)
    {
        glowTime += dt * 5;

        float glow = std::abs(std::sin(glowTime)) * 100 + 155;
        sf::Color glowColor(glow, 69, 0);

        for (int i = 0; i < NUM_LEVELS; i++)
        {
            if (i == level_index)
                buttons[i].setFillColor(glowColor);
            else
                buttons[i].setFillColor(sf::Color(70, 130, 180));
        }
    }

    void MoveUp()
{
    if (level_index > 0)
    {
        level_index--;
        hoverSound.play();
    }
}

void MoveDown()
{
    if (level_index < NUM_LEVELS - 1)
    {
        level_index++;
        hoverSound.play();
    }
}


    int Getlevel() const
    {
        return level_index;
    }

private:
    sf::Font font;
    sf::Text levelText[NUM_LEVELS];
    sf::RectangleShape buttons[NUM_LEVELS];
    int level_index;
    float glowTime;
    sf::SoundBuffer hoverBuffer;
    sf::SoundBuffer selectBuffer;
    sf::Sound hoverSound;
    sf::Sound selectSound;

};


class ModeMenu
{
public:
    ModeMenu(float width, float height)
    {
        if (!hoverBuffer.loadFromFile("hover.wav"))
        {
            cout << "Failed to load hover sound.\n";
        }
        if (!selectBuffer.loadFromFile("select.wav"))
        {
            cout << "Failed to load select sound.\n";
        }
        hoverSound.setBuffer(hoverBuffer);
        selectSound.setBuffer(selectBuffer);

        if (!font.loadFromFile("ByteBounce.ttf"))
        {
            cout << "Failed to load font." << endl;
        }

        modeNames[0] = "Single Player";
        modeNames[1] = "Multiplayer";
        modeNames[2] = "Back";

        for (int i = 0; i < 3; i++)
        {
            modeText[i].setFont(font);
            modeText[i].setString(modeNames[i]);
            modeText[i].setCharacterSize(50);
            modeText[i].setStyle(sf::Text::Bold);
            modeText[i].setFillColor(sf::Color::White);

            sf::FloatRect textBounds = modeText[i].getLocalBounds();
            modeText[i].setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);

            float posX = width / 2;
            float posY = height * 0.4f + i * (height * 0.15f);
            modeText[i].setPosition(posX, posY);

            buttons[i].setSize(sf::Vector2f(textBounds.width + 60, textBounds.height + 40));
            buttons[i].setOrigin(buttons[i].getSize() / 2.0f);
            buttons[i].setPosition(posX, posY);
            buttons[i].setOutlineThickness(4);
            buttons[i].setOutlineColor(sf::Color::Black);
        }

        title.setFont(font);
        title.setString("XONIX");
        title.setCharacterSize(100);
        title.setFillColor(sf::Color::Cyan);

        sf::FloatRect titleBounds = title.getLocalBounds();
        title.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top + titleBounds.height / 2.0f);
        title.setPosition(width / 2, height * 0.15f);

        menu_index = 0;
        glowTime = 0.f;
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(title);
        for (int i = 0; i < 3; i++)
        {
            window.draw(buttons[i]);
            window.draw(modeText[i]);
        }
    }

    void update(float dt)
    {
        glowTime += dt * 5;

        float glow = std::abs(std::sin(glowTime)) * 100 + 155;
        sf::Color glowColor(glow, 69, 0);

        for (int i = 0; i < 3; i++)
        {
            if (i == menu_index)
                buttons[i].setFillColor(glowColor);
            else
                buttons[i].setFillColor(sf::Color(70, 130, 180));
        }
    }

    void MoveUp()
    {
        if (menu_index > 0)
        {
            menu_index--;
            hoverSound.play();
        }
    }

    void MoveDown()
    {
        if (menu_index < 2)
        {
            menu_index++;
            hoverSound.play();
        }
    }

    int GetSelectedMode() const
    {
        return menu_index;
    }

private:
    sf::Font font;
    sf::Text modeText[3];
    sf::RectangleShape buttons[3];
    std::string modeNames[3];
    int menu_index;
    sf::Text title;
    float glowTime;
    sf::SoundBuffer hoverBuffer;
    sf::SoundBuffer selectBuffer;
    sf::Sound hoverSound;
    sf::Sound selectSound;
};



class MultiplayerMenu
{
public:
    MultiplayerMenu(float width, float height)
    {
        if (!hoverBuffer.loadFromFile("hover.wav"))
        {
            cout << "Failed to load hover sound.\n";
        }
        if (!selectBuffer.loadFromFile("select.wav"))
        {
            cout << "Failed to load select sound.\n";
        }
        hoverSound.setBuffer(hoverBuffer);
        selectSound.setBuffer(selectBuffer);

        if (!font.loadFromFile("ByteBounce.ttf"))
        {
            cout << "Failed to load font." << endl;
        }

        menuNames[0] = "Online";
        menuNames[1] = "Offline";
        menuNames[2] = "Back";

        for (int i = 0; i < 3; i++)
        {
            menuText[i].setFont(font);
            menuText[i].setString(menuNames[i]);
            menuText[i].setCharacterSize(50);
            menuText[i].setStyle(sf::Text::Bold);
            menuText[i].setFillColor(sf::Color::White);

            sf::FloatRect textBounds = menuText[i].getLocalBounds();
            menuText[i].setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);

            float posX = width / 2;
            float posY = height * 0.4f + i * (height * 0.15f);
            menuText[i].setPosition(posX, posY);

            buttons[i].setSize(sf::Vector2f(textBounds.width + 60, textBounds.height + 40));
            buttons[i].setOrigin(buttons[i].getSize() / 2.0f);
            buttons[i].setPosition(posX, posY);
            buttons[i].setOutlineThickness(4);
            buttons[i].setOutlineColor(sf::Color::Black);
        }

        title.setFont(font);
        title.setString("Multiplayer");
        title.setCharacterSize(80);
        title.setFillColor(sf::Color::Cyan);

        sf::FloatRect titleBounds = title.getLocalBounds();
        title.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top + titleBounds.height / 2.0f);
        title.setPosition(width / 2, height * 0.15f);

        menu_index = 0;
        glowTime = 0.f;
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(title);
        for (int i = 0; i < 3; i++)
        {
            window.draw(buttons[i]);
            window.draw(menuText[i]);
        }
    }

    void update(float dt)
    {
        glowTime += dt * 5;
        float glow = std::abs(std::sin(glowTime)) * 100 + 155;
        sf::Color glowColor(glow, 69, 0);

        for (int i = 0; i < 3; i++)
        {
            if (i == menu_index)
                buttons[i].setFillColor(glowColor);
            else
                buttons[i].setFillColor(sf::Color(70, 130, 180));
        }
    }

    void MoveUp()
    {
        if (menu_index > 0)
        {
            menu_index--;
            hoverSound.play();
        }
    }

    void MoveDown()
    {
        if (menu_index < 2)
        {
            menu_index++;
            hoverSound.play();
        }
    }

    int GetSelectedOption() const
    {
        return menu_index;
    }

private:
    sf::Font font;
    sf::Text menuText[3];
    sf::RectangleShape buttons[3];
    std::string menuNames[3];
    int menu_index;
    sf::Text title;
    float glowTime;
    sf::SoundBuffer hoverBuffer;
    sf::SoundBuffer selectBuffer;
    sf::Sound hoverSound;
    sf::Sound selectSound;
};

int getEnemyCountFromLevel(int level) 
{
switch (level) 
{
case 0: return 2;  //for easy level just 2 enemies
case 1: return 3;  //for meduim level 3 enemies
case 2: return 4;  //for hard level 4 enemies
default: return 2;  //user does not select level then deafult easy levl should follow
}
}


bool isRoomIDUsed(int roomID, int usedIDs[], int count) {
    for (int i = 0; i < count; i++) {
        if (usedIDs[i] == roomID)
            return true;
    }
    return false;
}

void showLoadingScreen(int id1, int id2) 
{
    sf::RenderWindow loadingWindow(sf::VideoMode(800, 600), "Creating Room");

    sf::Font font;
    if (!font.loadFromFile("ByteBounce.ttf")) 
    {
        cout<<"Font loading failed!"<<endl;
        return;
    }

    // Title text
    string msg="Creating room for Player ID = " + std::to_string(id1) +
                      " and Player ID = " + std::to_string(id2);
    sf::Text loadingText(msg,font,28);
    loadingText.setFillColor(sf::Color::White);
    sf::FloatRect textRect = loadingText.getLocalBounds();
    loadingText.setOrigin(textRect.width/2,textRect.height/2);
    loadingText.setPosition(400,150);

    // Progress bar background
    sf::RectangleShape progressBarBg(sf::Vector2f(600, 30));
    progressBarBg.setFillColor(sf::Color(50, 50, 50));
    progressBarBg.setPosition(100, 300);

    // Progress bar fill
    sf::RectangleShape progressBar(sf::Vector2f(0, 30));
    progressBar.setFillColor(sf::Color(0,200,255));
    progressBar.setPosition(100,300);

    // Percentage text
    sf::Text percentText("0%",font,24);
    percentText.setFillColor(sf::Color::White);
    percentText.setPosition(370,340);

    sf::Clock clock;
    float duration=5.0f;

    // Prepare unique room ID
    const int MAX_IDS=10;
    int usedIDs[MAX_IDS];
    int count=0;

    ifstream infile("room.txt");
    string line;
    while (getline(infile, line) && count<MAX_IDS) 
    {
        size_t pos=line.find("Room ID:");
        if (pos!=string::npos) 
        {
            istringstream getID(line.substr(pos + 9));
            int existingID;
            getID>>existingID;
            usedIDs[count++]=existingID;
        }
    }
    infile.close();

    srand(static_cast<unsigned>(std::time(0)));
    int roomID;
    do 
    {
        roomID=rand()%9000+1000;
    }while (isRoomIDUsed(roomID, usedIDs, count));

    // Show loading screen
    while (loadingWindow.isOpen()) 
    {
        sf::Event event;
        while (loadingWindow.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
                loadingWindow.close();
        }

        float elapsed=clock.getElapsedTime().asSeconds();
        float progress=min(elapsed/duration,1.0f);
        int percent=static_cast<int>(progress * 100);

        progressBar.setSize(sf::Vector2f(600 * progress, 30));
        percentText.setString(std::to_string(percent) + "%");

        if (percent >= 100) 
        {
            loadingWindow.close();

            // Write room info
            std::ofstream outfile("room.txt", std::ios::app);
            if (outfile.is_open()) 
            {
                outfile<<"Room ID: "<<roomID
                      <<" | Player1 ID: "<<id1
                      <<" | Player2 ID: " <<id2<<endl;
                outfile.close();
            } 
            else 
            {
                cout<<"Failed to open room.txt"<<endl;
            }
        }

        loadingWindow.clear(sf::Color(20,20,30));
        loadingWindow.draw(loadingText);
        loadingWindow.draw(progressBarBg);
        loadingWindow.draw(progressBar);
        loadingWindow.draw(percentText);
        loadingWindow.display();
    }
}

class PopupWindow 
{
public:
    static string getInput(const string& title, const string& promptText, const sf::Font& font) 
    {
        sf::RenderWindow inputWindow(sf::VideoMode(600,200), title);
        sf::Text prompt(promptText, font, 30);
        prompt.setFillColor(sf::Color::White);
        prompt.setPosition(50, 40);

        sf::Text inputText("", font, 30);
        inputText.setFillColor(sf::Color::Cyan);
        inputText.setPosition(250, 40);

        string input="";
        bool submitted=false;

        while(inputWindow.isOpen() && !submitted) 
        {
            sf::Event event;
            while(inputWindow.pollEvent(event)) 
            {
                if(event.type==sf::Event::Closed) 
                {
                    inputWindow.close();
                    break;
                }
                if(event.type==sf::Event::TextEntered) 
                {
                    if(event.text.unicode=='\b' && !input.empty()) 
                    {
                        input.pop_back();
                    } 
                    else if(event.text.unicode=='\r') 
                    {
                        submitted=true;
                    } 
                    else if(event.text.unicode<128 && event.text.unicode!='\r') 
                    {
                        input+=static_cast<char>(event.text.unicode);
                    }
                    inputText.setString(input);
                }
            }
            inputWindow.clear(sf::Color::Black);
            inputWindow.draw(prompt);
            inputWindow.draw(inputText);
            inputWindow.display();
        }
        inputWindow.close();
        return input;
    }

    static void showMessage(const string& title, const string& message, const sf::Font& font, sf::Color color, float duration=2.5f) {
        sf::RenderWindow msgWindow(sf::VideoMode(500,150), title);
        sf::Text msgText(message, font, 24);
        msgText.setFillColor(color);
        msgText.setPosition(50, 50);

        sf::Clock clock;
        while(msgWindow.isOpen()) {
            sf::Event event;
            while(msgWindow.pollEvent(event)) {
                if(event.type==sf::Event::Closed || clock.getElapsedTime().asSeconds()>duration) {
                    msgWindow.close();
                }
            }
            msgWindow.clear();
            msgWindow.draw(msgText);
            msgWindow.display();
        }
    }
};



int main()
{
int loginStatus=loginpage(); //login.cpp
 if (loginStatus==2)
 {
  int regStatus=registerpage();  //register.cpp
  if (regStatus!=1)
  {
   cout<<"Registration failed or canceled"<<endl;
   return 0;
  }
 }
  else if(loginStatus!=1)
  {
  cout<<"Login failed or canceled"<<endl;
  return 0;
  }

// window after login and register
 sf::RenderWindow window(sf::VideoMode(800, 700), "XONIX MENU", sf::Style::Default);
 sf::Texture background_t;
  if(!background_t.loadFromFile("1.png"))
 {
  cout<<"Failed to load background image"<<endl;
  return -1;
 }

 ThemeInventory inventory;
 inventory.loadThemesFromFile("inventory.txt");

Theme themes[10];
int count=inventory.getThemes(themes);

sf::Font font;
font.loadFromFile("ByteBounce.ttf");

sf::Sprite backgroundSprite(background_t);

Menu menu(window.getSize().x, window.getSize().y);
LevelMenu levelMenu(window.getSize().x, window.getSize().y);
ModeMenu modeMenu(window.getSize().x, window.getSize().y);
MultiplayerMenu multiplayerMenu(window.getSize().x, window.getSize().y);
ThemeMenu themeMenu(window.getSize().x, window.getSize().y,inventory);

bool inModeSelection=true;
bool inLevelSelection=false;
bool inMainMenu=false;
bool inleaderboardmenu=false;
bool inMultiplayerMenu=true;
bool inThemeSelection=true;
Leaderboard lb;

int level=0;
int final_score=0;

while (window.isOpen())
{
sf::Event event;
while (window.pollEvent(event))
{
if (event.type==sf::Event::Closed)
window.close();


    if (event.type==sf::Event::KeyPressed) 
    {
        if (inModeSelection) 
        {
           if (event.key.code==sf::Keyboard::Up)
            modeMenu.MoveUp();
           else if(event.key.code==sf::Keyboard::Down)
            modeMenu.MoveDown();
           else if(event.key.code==sf::Keyboard::Enter) 
          {
           int selectedMode=modeMenu.GetSelectedMode();
           if (selectedMode==0)  // Single Player
           {
            inModeSelection = false;
            inMainMenu = true;
           }

    else if (selectedMode == 1)  // Multiplayer
    {
      inModeSelection = false;
      bool inMultiplayerMenu = true;
      MultiplayerMenu multiplayerMenu(window.getSize().x, window.getSize().y);

      while (inMultiplayerMenu && window.isOpen()) 
     {
       sf::Event mpEvent;
       while (window.pollEvent(mpEvent)) 
    {
        if (mpEvent.type==sf::Event::Closed) 
        {
            window.close();
        }
        if (mpEvent.type==sf::Event::KeyPressed) 
        {
            if (mpEvent.key.code==sf::Keyboard::Up) 
            {
                multiplayerMenu.MoveUp();
            } 
            else if (mpEvent.key.code==sf::Keyboard::Down) 
            {
                multiplayerMenu.MoveDown();
            } 
            else if (mpEvent.key.code==sf::Keyboard::Enter) 
            {
                int choice=multiplayerMenu.GetSelectedOption();
                 if (choice == 0) 
{
    cout<<"Online Multiplayer Selected"<<endl;

    // Load players and friends
    load_players(); // from friend_system.h
    string current_user;
ifstream active("activeuser.txt");
if(active.is_open())
{
    active >> current_user; // Read the username only, ignore score
    active.close();
}
else
{
    cout<<"Error: Could not open activeuser.txt"<<endl;
    inMultiplayerMenu = false;
    break;
}
 // or get this from your login/user system

    vector<string>friendUsernames;
    string file_path = "profiles/" + current_user + ".txt";
    ifstream profile(file_path.c_str());
    string line;
    while (getline(profile, line))
    {
        if (line.find("Friend:") == 0)
        {
            string friend_username = line.substr(7); // Skip "Friend:"
            friendUsernames.push_back(friend_username);
        }
    }
    profile.close();

   if(friendUsernames.empty())
{
    cout<<"You have no friends to play with. Redirecting to Friend Menu..."<<endl;
    show_friend_menu(window, current_user);

    // Reload the friends list after possibly adding friends
    friendUsernames.clear();
    ifstream updated_profile(file_path.c_str());
    while (getline(updated_profile, line))
    {
        if (line.find("Friend:") == 0)
        {
            string friend_username = line.substr(7); // Skip "Friend:"
            friendUsernames.push_back(friend_username);
        }
    }
    updated_profile.close();

    if(friendUsernames.empty())
    {
        cout<<"Still no friends found. Returning to main menu."<<endl;
        inMultiplayerMenu = false;
        break;
    }
}

    else
    {
        // Choose friend from friend list
        int selected_index = 0;
        while (window.isOpen())
        {
            Event e;
            while (window.pollEvent(e))
            {
                if (e.type == Event::Closed)
                    window.close();
                if (e.type == Event::KeyPressed)
                {
                    if (e.key.code == Keyboard::Up)
                        selected_index = (selected_index + friendUsernames.size() - 1) % friendUsernames.size();
                    if (e.key.code == Keyboard::Down)
                        selected_index = (selected_index + 1) % friendUsernames.size();
                    if (e.key.code == Keyboard::Enter)
                    {
                        string friend_username = friendUsernames[selected_index];
                        int id1 = find_player_index(current_user);
                        int id2 = find_player_index(friend_username);
                        if(id1 != -1 && id2 != -1)
                        {
                            showLoadingScreen(id1, id2);
                            multiplayer(id1, id2);
                        }
                        else
                        {
                            cout<<"Failed to find player IDs."<<endl;
                        }
                        window.close();
                        inMultiplayerMenu = false;
                        break;
                    }
                    if (e.key.code == Keyboard::Escape)
                    {
                        inMultiplayerMenu = false;
                        break;
                    }
                }
            }

            window.clear();
            Font font;
            font.loadFromFile("arial.ttf");
            display_text(window, font, "Select Friend to Play With:", 100, 30, Color::White, 28);
            for (int i = 0; i < friendUsernames.size(); i++)
            {
                Color c = (i == selected_index ? Color::Green : Color::White);
                display_text(window, font, friendUsernames[i], 100, 80 + i * 30, c, 24);

            }
            window.display();
        }
    }

    inMultiplayerMenu = false;
}
 
                else if (choice == 1) 
                {
                   cout<<"Offline Multiplayer Selected"<<endl;
                   int id1, id2;
                   getTopTwoPlayers(id1, id2);

                   if(id1!=-1 && id2!=-1) 
                   {
                      cout<<"Top players: "<<id1<<" and "<<id2<<endl; // if enough new players avaible then only play multipayer
                      showLoadingScreen(id1, id2);
                      multiplayer(id1,id2);
                      
                   } 
                    else 
                   {
                     cout<<"Not enough players available."<<endl;
                   }

                    window.close();
                    inMultiplayerMenu=false;
                }
                else if (choice == 2) 
                {
                    inMultiplayerMenu=false;
                    inModeSelection=true;
                }
            }
        }
    }
    
    sf::Clock clock;
float dt = clock.restart().asSeconds();
    window.clear();
    multiplayerMenu.draw(window);
    multiplayerMenu.update(dt);
    window.display();
}

}


    else if (selectedMode==2)   // BAck button will take back to login window
    { 
      cout<<"Going back to login window..."<<endl;
      window.close();
      main(); //back to login
    }
}


}


        else if (inLevelSelection) 
        {
            if (event.key.code==sf::Keyboard::Up)
            {
                levelMenu.MoveUp();
            }    
            else if (event.key.code==sf::Keyboard::Down)
            {
                levelMenu.MoveDown();
            }    
            else if (event.key.code==sf::Keyboard::Enter) 
            {
                level=levelMenu.Getlevel();
                cout<<"Level selected: "<<level<<endl;
                inLevelSelection=false;
                inMainMenu=true;
            }
        }
        
        if (inThemeSelection) 
        {
          if(event.type == sf::Event::KeyPressed) 
          {
           switch (event.key.code) 
           {
            case sf::Keyboard::Up:
              themeMenu.MoveUp();
               break;
            case sf::Keyboard::Down:
               themeMenu.MoveDown();
               break;
            case sf::Keyboard::Enter: 
            {
              int selected=themeMenu.get_themeid();
              Theme* sel_t=inventory.searchThemeById(themes[selected].getId());

            if (sel_t) 
            {
                int r,g,b;
                sel_t->getRGBCodes(r,g,b);
                themeMenu.setbackground_rgb(r,g,b);
                themeMenu.applyBackground(window);
                cout<<"Selected Theme ID: "<<sel_t->getId()<<endl;
            }

              inThemeSelection=false;
              inMainMenu=true;
              break;
            }
            default:
              break;
    }
}

}

else if(inMainMenu)
{
if(event.key.code==sf::Keyboard::Up)
{
menu.MoveUp();
}
else if(event.key.code==sf::Keyboard::Down)
{
menu.MoveDown();
}
else if(event.key.code==sf::Keyboard::Enter)
{
menu.select();
int selectedIndex=menu.GetPressedItem();
if(selectedIndex==0)
{
window.close();
int enemyCount=getEnemyCountFromLevel(level);
final_score=runXonixGame(enemyCount,R,G,B,nullptr);
sf::RenderWindow inputWindow(sf::VideoMode(600,200),"Enter Username");
sf::Font font;
font.loadFromFile("ByteBounce.ttf");


        sf::Text prompt("Enter Username:",font,30);
        prompt.setFillColor(sf::Color::White);
        prompt.setPosition(50,40);

        sf::Text inputText("",font,30);
        inputText.setFillColor(sf::Color::Cyan);
        inputText.setPosition(300,40);

        string e_username="";
        bool submitted=false;

        while(inputWindow.isOpen()&&!submitted) 
        {
            sf::Event event;
            while(inputWindow.pollEvent(event)) 
            {
                if(event.type==sf::Event::Closed) 
                {
                    inputWindow.close();
                    break;
                }

                if(event.type==sf::Event::TextEntered) 
                {
                    if(event.text.unicode=='\b'&&!e_username.empty()) 
                    {
                        e_username.pop_back();
                    } 
                    else if(event.text.unicode=='\r') 
                    {
                        submitted=true;
                    } 
                    else if(event.text.unicode<128&&event.text.unicode!='\r') 
                    {
                        e_username+=static_cast<char>(event.text.unicode);
                    }
                    inputText.setString(e_username);
                }
            }

            inputWindow.clear(sf::Color::Black);
            inputWindow.draw(prompt);
            inputWindow.draw(inputText);
            inputWindow.display();
        }
    




    inputWindow.close();

// Verify username and get userID from users.txt
ifstream userFile("users.txt");
string line;
string e_id;
bool found=false;

while(std::getline(userFile,line)) 
{
 size_t idPos=line.find("ID: ");
 size_t unamePos=line.find("| Username: ");
 size_t passPos=line.find("| Password: ");

if(idPos!=string::npos && unamePos!=string::npos && passPos!=string::npos) 
{ 
    string id=line.substr(idPos+4,unamePos-(idPos+4));
    string username=line.substr(unamePos+12,passPos-(unamePos+12));

    id.erase(id.find_last_not_of(" \n\r\t")+1);
    username.erase(username.find_last_not_of(" \n\r\t")+1);

    if(username==e_username) 
    {
        e_id=id;
        found=true;
        break;
    }
}

}


userFile.close();

if(found) 
{
lb.updatePlayerScore(e_id.c_str(), final_score);

sf::RenderWindow lbWindow(sf::VideoMode(800, 600), "Leaderboard");
lb.displayLeaderboard(lbWindow);
}
else 
{
    sf::RenderWindow bugwindow(sf::VideoMode(500,150),"Error");
    sf::Text errorText("Username not found!",font,30);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(100, 50);

    sf::Clock clock;
    while (bugwindow.isOpen()) 
    {
        sf::Event event;
        while (bugwindow.pollEvent(event)) 
        {
            if (event.type==sf::Event::Closed || clock.getElapsedTime().asSeconds()>3) 
            {
                bugwindow.close();
            }
        }

        bugwindow.clear();
        bugwindow.draw(errorText);
        bugwindow.display();
    }
}


}

  else if(selectedIndex==1) // Resume Game
{
    // Create a window to input Save ID
    sf::RenderWindow inputWindow(sf::VideoMode(600,200),"Enter Save ID");
    sf::Font font;
    font.loadFromFile("ByteBounce.ttf");

    sf::Text prompt("Enter Save ID:",font,30);
    prompt.setFillColor(sf::Color::White);
    prompt.setPosition(50,40);

    sf::Text inputText("",font,30);
    inputText.setFillColor(sf::Color::Cyan);
    inputText.setPosition(250,40);

    string saveID="";
    bool submitted=false;

    while(inputWindow.isOpen() && !submitted) 
    {
        sf::Event event;
        while(inputWindow.pollEvent(event)) 
        {
            if(event.type==sf::Event::Closed) 
            {
                inputWindow.close();
                break;
            }

            if(event.type==sf::Event::TextEntered) 
            {
                if(event.text.unicode=='\b' && !saveID.empty()) 
                {
                    saveID.pop_back();
                } 
                else if(event.text.unicode=='\r') 
                {
                    submitted=true;
                } 
                else if(event.text.unicode<128 && event.text.unicode!='\r') 
                {
                    saveID+=static_cast<char>(event.text.unicode);
                }
                inputText.setString(saveID);
            }
        }

        inputWindow.clear(sf::Color::Black);
        inputWindow.draw(prompt);
        inputWindow.draw(inputText);
        inputWindow.display();
    }

    inputWindow.close();

    GameStructure resumedGame;
    if(resumedGame.loadFromFile(saveID)) 
    {
        // Show confirmation window
        sf::RenderWindow confirmWindow(sf::VideoMode(400,150), "Success");
        sf::Text successText("Save found. Resuming game...", font, 24);
        successText.setFillColor(sf::Color::Green);
        successText.setPosition(30,50);

        sf::Clock clock;
        while(confirmWindow.isOpen()) 
        {
            sf::Event event;
            while(confirmWindow.pollEvent(event)) 
            {
                if(event.type==sf::Event::Closed || clock.getElapsedTime().asSeconds()>2.5f)
                {
                    confirmWindow.close();
                }
            }

            confirmWindow.clear();
            confirmWindow.draw(successText);
            confirmWindow.display();
        }

        int enemyCount=getEnemyCountFromLevel(level); // Assuming GameStructure has level
        final_score=runXonixGame(enemyCount,R,G,B, &resumedGame);

        // Resume asks for username again
        sf::RenderWindow userInput(sf::VideoMode(600,200),"Enter Username");
        sf::Text userPrompt("Enter Username:",font,30);
        userPrompt.setFillColor(sf::Color::White);
        userPrompt.setPosition(50,40);

        sf::Text userText("",font,30);
        userText.setFillColor(sf::Color::Cyan);
        userText.setPosition(300,40);

        string e_username="";
        bool submittedU=false;

        while(userInput.isOpen() && !submittedU) 
        {
            sf::Event event;
            while(userInput.pollEvent(event)) 
            {
                if(event.type==sf::Event::Closed) 
                {
                    userInput.close();
                    break;
                }

                if(event.type==sf::Event::TextEntered) 
                {
                    if(event.text.unicode=='\b' && !e_username.empty()) 
                    {
                        e_username.pop_back();
                    } 
                    else if(event.text.unicode=='\r') 
                    {
                        submittedU=true;
                    } 
                    else if(event.text.unicode<128 && event.text.unicode!='\r') 
                    {
                        e_username+=static_cast<char>(event.text.unicode);
                    }
                    userText.setString(e_username);
                }
            }

            userInput.clear(sf::Color::Black);
            userInput.draw(userPrompt);
            userInput.draw(userText);
            userInput.display();
        }

        userInput.close();

        // Verify username
        ifstream userFile("users.txt");
        string line;
        string e_id;
        bool found=false;

        while(getline(userFile,line)) 
        {
            size_t idPos=line.find("ID: ");
            size_t unamePos=line.find("| Username: ");
            size_t passPos=line.find("| Password: ");

            if(idPos!=string::npos && unamePos!=string::npos && passPos!=string::npos) 
            {
                string id=line.substr(idPos+4,unamePos-(idPos+4));
                string username=line.substr(unamePos+12,passPos-(unamePos+12));

                id.erase(id.find_last_not_of(" \n\r\t")+1);
                username.erase(username.find_last_not_of(" \n\r\t")+1);

                if(username==e_username) 
                {
                    e_id=id;
                    found=true;
                    break;
                }
            }
        }

        userFile.close();

        if(found) 
        {
            lb.updatePlayerScore(e_id.c_str(), final_score);
            sf::RenderWindow lbWindow(sf::VideoMode(800, 600), "Leaderboard");
            lb.displayLeaderboard(lbWindow);
        }
        else 
        {
            sf::RenderWindow errorWindow(sf::VideoMode(500,150),"Error");
            sf::Text errorText("Username not found!",font,30);
            errorText.setFillColor(sf::Color::Red);
            errorText.setPosition(100, 50);

            sf::Clock clock;
            while (errorWindow.isOpen()) 
            {
                sf::Event event;
                while (errorWindow.pollEvent(event)) 
                {
                    if (event.type==sf::Event::Closed || clock.getElapsedTime().asSeconds()>3) 
                    {
                        errorWindow.close();
                    }
                }

                errorWindow.clear();
                errorWindow.draw(errorText);
                errorWindow.display();
            }
        }
    } 
    else 
    {
        // Show error if save not found
        sf::RenderWindow errorWindow(sf::VideoMode(500,150),"Save Not Found");
        sf::Text errorText(("Save not found with ID: "+saveID).c_str(), font, 24);
        errorText.setFillColor(sf::Color::Red);
        errorText.setPosition(50, 50);

        sf::Clock clock;
        while(errorWindow.isOpen()) 
        {
            sf::Event event;
            while(errorWindow.pollEvent(event)) 
            {
                if(event.type==sf::Event::Closed || clock.getElapsedTime().asSeconds()>3)
                {
                    errorWindow.close();
                }
            }

            errorWindow.clear();
            errorWindow.draw(errorText);
            errorWindow.display();
        }
    }
}



else if(selectedIndex==2) 
{
    inLevelSelection=true;
} 
 

else if(selectedIndex==4) 
{
    inThemeSelection = true;
} 
else if(selectedIndex == 3) 
{ 
  sf::RenderWindow leaderb_window(sf::VideoMode(800, 600), "Leaderboard");

  lb.displayLeaderboard(leaderb_window);

  while(leaderb_window.isOpen()) 
  {
    sf::Event event;
    while(leaderb_window.pollEvent(event)) 
    {
        if(event.type==sf::Event::Closed) 
        {
            leaderb_window.close(); 
        }
    }
    leaderb_window.clear();
    lb.displayLeaderboard(leaderb_window);
    leaderb_window.display();
  }
}


  else if (selectedIndex==5) 
  {
    cout<<"Logged out successfully."<<endl;
    window.close();
    main(); 
  }
  else if (selectedIndex==6) 
  { 
    inMainMenu=false;
    inModeSelection=true;
  }

}


}


  }


} 

sf::Clock deltaClock;
float dt = deltaClock.restart().asSeconds();

window.clear();
window.draw(backgroundSprite);

if (inModeSelection) 
{
 modeMenu.draw(window);  
 modeMenu.update(dt);
}  
else if(inLevelSelection) 
{
 levelMenu.draw(window);
 levelMenu.update(dt);
} 
else if(inThemeSelection) 
{
 themeMenu.draw(window);  
}  
else if(inMainMenu) 
{
menu.draw(window);
menu.update(dt); 
}
window.display();


}

return 0;

}


