#include "game.h"
#include <SFML/Graphics.hpp>
#include <time.h>
#include<cstring>
#include<string>
#include<sstream>
#include<fstream>
#include<iostream>
#include"gamestructure.h"
#include <ctime>
using namespace sf;
using namespace std;

const int M = 25;
const int N = 40;
int grid[M][N] = {0};
int ts = 18; //tile size


struct Enemy
{int x,y,dx,dy;
bool paused=false;

  Enemy()
   {
    x=y=300;
    dx=4-rand()%8;
    dy=4-rand()%8;
   }

  void move()
   { 
    x+=dx; if (grid[y/ts][x/ts]==1) {dx=-dx; x+=dx;}
    y+=dy; if (grid[y/ts][x/ts]==1) {dy=-dy; y+=dy;}
   }
};

void drop(int y,int x)
{
  if (grid[y][x]==0) grid[y][x]=-1;
  if (grid[y-1][x]==0) drop(y-1,x);
  if (grid[y+1][x]==0) drop(y+1,x);
  if (grid[y][x-1]==0) drop(y,x-1);
  if (grid[y][x+1]==0) drop(y,x+1);
}



int runXonixGame(int enemyCount, int R, int G, int B, GameStructure* resume = nullptr)
{
    srand(time(0));

    RenderWindow window(VideoMode(N*ts, M*ts), "Xonix Game!");
    window.setFramerateLimit(60);
    
    cout<<"game.cpp"<<R<<" "<<G<<" "<<B<<endl;
    
    Background bg(Color(R,G,B));

    Texture t1,t2,t3;
    t1.loadFromFile("tiles.png");
    t2.loadFromFile("gameover.png");
    t3.loadFromFile("enemy.png");

    Sprite sTile(t1), sGameover(t2), sEnemy(t3);
    sGameover.setPosition(100,100);
    sEnemy.setOrigin(20,20);

    Enemy a[10];

    bool Game=true;
    int x=0, y=0, dx=0, dy=0;
    float timer=0, delay=0.07; 
    Clock clock;

    // Score and reward counters
    int score = 0;
    int rewardCount = 0;
    int capturedTiles = 0;
    bool powerUpActive = false;
    float powerUpTimer = 0;
    int powerUpInventory = 0; // Tracks the number of power-ups in the inventory
    
    if (resume != nullptr) {
        x = resume->x;
        y = resume->y;
        dx = resume->dx;
        dy = resume->dy;
        score = resume->score;
        rewardCount = resume->rewardCount;
        capturedTiles = resume->capturedTiles;
        powerUpInventory = resume->powerUpInventory;
        powerUpActive = resume->powerUpActive;
        powerUpTimer = resume->powerUpTimer;
        for (int i=0;i<M;i++)
            for (int j=0;j<N;j++)
                grid[i][j] = resume->grid[i][j];
    }

    for (int i=0;i<M;i++)
     for (int j=0;j<N;j++)
      if (i==0 || j==0 || i==M-1 || j==N-1)  grid[i][j]=1;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
               
            if (e.type == Event::KeyPressed)
             if (e.key.code==Keyboard::Escape)
               {
                for (int i=1;i<M-1;i++)
                 for (int j=1;j<N-1;j++)
                   grid[i][j]=0;

                x=10;y=0;
                Game=true;
               }
               
               if (e.key.code == Keyboard::Q)
        {
            window.close(); // Close the game window
        }

            // Use the power-up (when 'P' is pressed)
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::P)
            {
                if (powerUpInventory > 0 && !powerUpActive)
                {
                    powerUpActive = true;
                    powerUpTimer = 3;  // Set the power-up duration to 3 seconds
                    powerUpInventory--; // Decrease the number of available power-ups
                }
            }
            
           else if (e.type == Event::KeyPressed && e.key.code == Keyboard::S)
    {
    GameStructure g;

    cout<<"Enter a Save ID (no spaces): ";
    cin>>g.saveID;
    g.timestamp = std::time(nullptr);
    g.x = x; g.y = y; g.dx = dx; g.dy = dy;
    g.score = score; g.rewardCount = rewardCount;
    g.capturedTiles = capturedTiles;
    g.powerUpInventory = powerUpInventory;
    g.powerUpActive = powerUpActive;
    g.powerUpTimer = powerUpTimer;

    for (int i = 0; i < M; ++i)
        for (int j = 0; j < N; ++j)
            g.grid[i][j] = grid[i][j];

    g.saveToFile(); 


    sf::RenderWindow notifyWindow(sf::VideoMode(500, 150), "Game Saved");
    sf::Font font;
    font.loadFromFile("ByteBounce.ttf");

    sf::Text notify("Game Saved! Save ID:\n" + g.saveID, font, 24);
    notify.setFillColor(sf::Color::Green);
    notify.setPosition(50, 50);

    sf::Clock clock;
    while (notifyWindow.isOpen())
    {
        sf::Event event;
        while (notifyWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || clock.getElapsedTime().asSeconds() > 4)
                notifyWindow.close();
        }

        notifyWindow.clear(sf::Color::Black);
        notifyWindow.draw(notify);
        notifyWindow.display();
    }
}


        }


        if (Keyboard::isKeyPressed(Keyboard::Left)) {dx=-1;dy=0;};
        if (Keyboard::isKeyPressed(Keyboard::Right))  {dx=1;dy=0;};
        if (Keyboard::isKeyPressed(Keyboard::Up)) {dx=0;dy=-1;};
        if (Keyboard::isKeyPressed(Keyboard::Down)) {dx=0;dy=1;};
        
        
         if (!Game)
         {
           continue;
         }

        if (timer>delay)
        {
         x+=dx;
         y+=dy;

         if (x<0) x=0; if (x>N-1) x=N-1;
         if (y<0) y=0; if (y>M-1) y=M-1;

         if (grid[y][x]==2) Game=false;
         if (grid[y][x]==0) 
         {
             grid[y][x]=2;
             capturedTiles++;
         }
         timer=0;
        }

        // Handle enemy movement, but only if the power-up is not active
        if (!powerUpActive)
        {
            for (int i=0;i<enemyCount;i++) 
                a[i].move();
        }

        if (grid[y][x]==1)
          {
           dx=dy=0;

           for (int i=0;i<enemyCount;i++)
                drop(a[i].y/ts, a[i].x/ts);

           for (int i=0;i<M;i++)
             for (int j=0;j<N;j++)
              if (grid[i][j]==-1) grid[i][j]=0;
              else grid[i][j]=1;
          }

        for (int i=0;i<enemyCount;i++)
           if  (grid[a[i].y/ts][a[i].x/ts]==2) Game=false;

        // Handle bonus points and rewards
        if (capturedTiles > 10)
        {
            int multiplier = 2;
            if (rewardCount >= 3) multiplier = 4; // Double points after 3 occurrences, quadruple after 5
            score += capturedTiles * multiplier;
            rewardCount++;
            capturedTiles = 0;
        }
        else
        {
            score += capturedTiles;
            capturedTiles = 0;
        }

        // Handle power-ups
        if (score >= 50 && powerUpInventory == 0) 
        {
            powerUpInventory++;  // Grant a power-up when the score reaches 50
        }
        if (score >= 70 && powerUpInventory == 1) 
        {
            powerUpInventory++;  // Grant another power-up at score 70
        }
        if (score >= 100 && powerUpInventory == 2) 
        {
            powerUpInventory++;  // And so on...
        }

        if (powerUpActive)
        {
            powerUpTimer -= time;
            if (powerUpTimer <= 0)
            {
                powerUpActive = false;
            }
        }

      /////////draw//////////
      window.clear();
bg.draw(window);

      for (int i=0;i<M;i++)
        for (int j=0;j<N;j++)
         {
            if (grid[i][j]==0) continue;
            if (grid[i][j]==1) sTile.setTextureRect(IntRect( 0,0,ts,ts));
            if (grid[i][j]==2) sTile.setTextureRect(IntRect(54,0,ts,ts));
            sTile.setPosition(j*ts,i*ts);
            window.draw(sTile);
         }

      sTile.setTextureRect(IntRect(36,0,ts,ts));
      sTile.setPosition(x*ts,y*ts);
      window.draw(sTile);

      sEnemy.rotate(10);
      for (int i=0;i<enemyCount;i++)
       {
        sEnemy.setPosition(a[i].x,a[i].y);
        window.draw(sEnemy);
       }


if (!Game)
{
    Font font;
    font.loadFromFile("arial.ttf");

    Text finalScoreText("Final Score: " + std::to_string(score), font, 30);
    finalScoreText.setFillColor(sf::Color::Red);
    finalScoreText.setPosition(100, 200);
    window.draw(finalScoreText);

    // Display restart instructions
    Text restartText("Press ESC to Restart", font, 24);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition(100, 250);
    window.draw(restartText);

    // Display quit instructions
    Text quitText("Press Q to Quit", font, 24);
    quitText.setFillColor(sf::Color::White);
    quitText.setPosition(100, 300);  // Adjust position as needed
    window.draw(quitText);
}
 

      // Display the score on the screen
      Font font;
      font.loadFromFile("arial.ttf"); // Ensure the font file is loaded
      Text scoreText("Score: " + std::to_string(score), font, 24);
      scoreText.setFillColor(sf::Color::White);
      scoreText.setPosition(10, 10); // Position the score on the top left
      window.draw(scoreText);

      // Display the power-up status
      if (powerUpActive)
      {
          Text powerUpText("Power-Up Active!", font, 24);
          powerUpText.setFillColor(sf::Color::Green);
          powerUpText.setPosition(10, 40);
          window.draw(powerUpText);
      }

      // Display remaining power-ups
      Text powerUpInventoryText("Power-Ups: " + std::to_string(powerUpInventory), font, 24);
      powerUpInventoryText.setFillColor(sf::Color::White);
      powerUpInventoryText.setPosition(10, 70);
      window.draw(powerUpInventoryText);
      
      window.display();
    }
    return score;
}

int multiplayer(int id1,int id2)
{
    srand(time(0));
    RenderWindow window(VideoMode(N * ts, M * ts + 60), "Xonix Multiplayer!");
    window.setFramerateLimit(60);

    Texture t1, t2, t3;
    t1.loadFromFile("tiles.png");
    t2.loadFromFile("gameover.png");
    t3.loadFromFile("enemy.png");

    Sprite sTile(t1), sGameover(t2), sEnemy(t3);
    sGameover.setPosition(100, 100);
    sEnemy.setOrigin(20, 20);

    int enemyCount = 4;
    Enemy a[10];

    // Initialize player positions and directions.
    int x1 = 10, y1 = 0, dx1 = 0, dy1 = 0, score1 = 0;
    int x2 = N - 10, y2 = M - 2, dx2 = 0, dy2 = 0, score2 = 0;

    // Power-up related variables
    int powerUps1 = 0, powerUps2 = 0;
    bool Game = true, p1Frozen = false, p2Frozen = false;
    float freezeTimer = 0;

    float timer = 0, delay = 0.07;
    Clock clock;

    // Initialize the grid
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            grid[i][j] = (i == 0 || j == 0 || i == M - 1 || j == N - 1);

    Font font;
    font.loadFromFile("arial.ttf");

    Text scoreText1, scoreText2, powerText1, powerText2;
    scoreText1.setFont(font);
    scoreText1.setCharacterSize(20);
    scoreText1.setFillColor(Color::White);
    scoreText1.setPosition(10, M * ts + 5); 

    powerText1.setFont(font);
    powerText1.setCharacterSize(14);
    powerText1.setFillColor(Color::Green);
    powerText1.setPosition(10, M * ts + 30);

    scoreText2.setFont(font);
    scoreText2.setCharacterSize(20);
    scoreText2.setFillColor(Color::White);
    scoreText2.setPosition(250, M * ts + 5);

    powerText2.setFont(font);
    powerText2.setCharacterSize(14);
    powerText2.setFillColor(Color::Green);
    powerText2.setPosition(250, M * ts + 30);


    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        if (p1Frozen || p2Frozen) freezeTimer += time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)
            {
                // Reset game variables
                for (int i = 1; i < M - 1; i++)
                    for (int j = 1; j < N - 1; j++)
                        grid[i][j] = 0;

                // Reset player positions and scores
                x1 = 10; y1 = 0; dx1 = dy1 = 0; score1 = 0; powerUps1 = 0;
                x2 = N - 10; y2 = M - 2; dx2 = dy2 = 0; score2 = 0; powerUps2 = 0;
                Game = true; p1Frozen = p2Frozen = false;
            }
        }

        // Handle player movement
        if (!p1Frozen) 
        {
            if (Keyboard::isKeyPressed(Keyboard::Left)) { dx1 = -1; dy1 = 0; }
            if (Keyboard::isKeyPressed(Keyboard::Right)) { dx1 = 1; dy1 = 0; }
            if (Keyboard::isKeyPressed(Keyboard::Up)) { dx1 = 0; dy1 = -1; }
            if (Keyboard::isKeyPressed(Keyboard::Down)) { dx1 = 0; dy1 = 1; }
        }

        if (!p2Frozen) 
        {
            if (Keyboard::isKeyPressed(Keyboard::A)) { dx2 = -1; dy2 = 0; }
            if (Keyboard::isKeyPressed(Keyboard::D)) { dx2 = 1; dy2 = 0; }
            if (Keyboard::isKeyPressed(Keyboard::W)) { dx2 = 0; dy2 = -1; }
            if (Keyboard::isKeyPressed(Keyboard::S)) { dx2 = 0; dy2 = 1; }
        }

        // Handle freeze power-up
        if (Keyboard::isKeyPressed(Keyboard::Space) && powerUps1 > 0 && !p1Frozen && !p2Frozen) 
        {
            powerUps1--;
            p1Frozen = p2Frozen = true;
            for (int i = 0; i < enemyCount; i++) a[i].paused = true;
            freezeTimer = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::LShift) && powerUps2 > 0 && !p1Frozen && !p2Frozen) 
        {
            powerUps2--;
            p1Frozen = p2Frozen = true;
            for (int i = 0; i < enemyCount; i++) a[i].paused = true;
            freezeTimer = 0;
        }

        // Check for freeze end
        if ((p1Frozen || p2Frozen) && freezeTimer > 3) 
        {
            p1Frozen = p2Frozen = false;
            for (int i = 0; i < enemyCount; i++) a[i].paused = false;
        }

        // Main game logic
        if (!Game) continue;

        if (timer > delay)
        {
            // Update player positions
            x1 += dx1; y1 += dy1;
            x2 += dx2; y2 += dy2;

            // Ensure players don't move off the grid
            x1 = std::clamp(x1, 0, N - 1);
            y1 = std::clamp(y1, 0, M - 1);
            x2 = std::clamp(x2, 0, N - 1);
            y2 = std::clamp(y2, 0, M - 1);

            // Player collision logic
            if (x1 == x2 && y1 == y2) 
            {
                Game = false; // Both players collide at the same spot
            } 
            else 
            {
                if (grid[y1][x1] == 0) { grid[y1][x1] = 2; score1++; }
                if (grid[y2][x2] == 0) { grid[y2][x2] = 2; score2++; }
            }

            // Update power-up collection logic
            if (score1 >= 50 && (score1 - 50) % 30 == 0) powerUps1++;
            if (score2 >= 50 && (score2 - 50) % 30 == 0) powerUps2++;

            timer = 0;
        }

        // Move enemies
        for (int i = 0; i < enemyCount; i++) if (!a[i].paused) a[i].move();
        
        if (grid[y1][x1] == 1 || grid[y2][x2] == 1)
        {
            dx1 = dy1 = dx2 = dy2 = 0;
            for (int i = 0; i < enemyCount; i++)
                drop(a[i].y / ts, a[i].x / ts);
            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    grid[i][j] = (grid[i][j] == -1 ? 0 : 1);
        }


        // Enemy collision check
        for (int i = 0; i < enemyCount; i++)
        {
            float ex = a[i].x, ey = a[i].y;
            float px1 = x1 * ts + ts / 2, py1 = y1 * ts + ts / 2;
            float px2 = x2 * ts + ts / 2, py2 = y2 * ts + ts / 2;

            if (abs(ex - px1) < 18 && abs(ey - py1) < 18) Game = false;
            if (abs(ex - px2) < 18 && abs(ey - py2) < 18) Game = false;
        }

        // Drawing logic
        window.clear();

        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
            {
                if (grid[i][j] == 0) continue;
                if (grid[i][j] == 1) sTile.setTextureRect(IntRect(0, 0, ts, ts));
                if (grid[i][j] == 2) sTile.setTextureRect(IntRect(54, 0, ts, ts));
                sTile.setPosition(j * ts, i * ts);
                window.draw(sTile);
            }

        sTile.setTextureRect(IntRect(36, 0, ts, ts));
        sTile.setPosition(x1 * ts, y1 * ts);
        window.draw(sTile);

        sTile.setTextureRect(IntRect(18, 0, ts, ts));
        sTile.setPosition(x2 * ts, y2 * ts);
        window.draw(sTile);

        sEnemy.rotate(10);
        for (int i = 0; i < enemyCount; i++)
        {
            sEnemy.setPosition(a[i].x, a[i].y);
            window.draw(sEnemy);
        }

        std::stringstream ss1, ss2;
        ss1 << score1;
        ss2 << score2;

        
        scoreText1.setString("Player " + std::to_string(id1) + " Score: " + ss1.str());
        scoreText2.setString("Player " + std::to_string(id2) + " Score: " + ss2.str());

        
        std::stringstream ss3, ss4;
        ss3 << powerUps1;
        ss4 << powerUps2;

        powerText1.setString("P1 Power-ups: " + ss3.str());
        powerText2.setString("P2 Power-ups: " + ss4.str());

        window.draw(scoreText1);
        window.draw(scoreText2);
        window.draw(powerText1);
        window.draw(powerText2);
if(!Game)
{

            // Winner announcement
if (!Game)
{
    if (score1 > score2)
    {
        std::stringstream winner;
        winner << "Player 1 Wins with " << score1 << " points!";
        Text winnerText;
        winnerText.setFont(font);
        winnerText.setCharacterSize(30);
        winnerText.setFillColor(Color::Green);
        winnerText.setString(winner.str());
        winnerText.setPosition(N * ts / 4, M * ts / 2);
        window.clear();
        window.draw(winnerText);
        window.display();
        sleep(seconds(3));  // Pause for 3 seconds to show the winner
    }
    else if (score2 > score1)
    {
        std::stringstream winner;
        winner << "Player 2 Wins with " << score2 << " points!";
        Text winnerText;
        winnerText.setFont(font);
        winnerText.setCharacterSize(30);
        winnerText.setFillColor(Color::Green);
        winnerText.setString(winner.str());
        winnerText.setPosition(N * ts / 4, M * ts / 2);
        window.clear();
        window.draw(winnerText);
        window.display();
        sleep(seconds(3));  // Pause for 3 seconds to show the winner
    }
    else
    {
        Text drawText;
        drawText.setFont(font);
        drawText.setCharacterSize(30);
        drawText.setFillColor(Color::Yellow);
        drawText.setString("It's a Draw!");
        drawText.setPosition(N * ts / 3, M * ts / 2);
        window.clear();
        window.draw(drawText);
        window.display();
        sleep(seconds(3));  // Pause for 3 seconds to show the result
    }
}

        
}
        window.display();
    }
    

    return 0;
}

