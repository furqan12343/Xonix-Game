#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include<SFML/Graphics.hpp>
#include<iostream>
#include<fstream>
#include<sstream>
#include<cstring>
using namespace std;

const int MAX_PLAYERS=10;

struct Player
{
    char id[50];
    int score;
    
    Player():score(0)
    {
      id[0]='\0';
    }
    Player(const char* i,int s):score(s)
    {
      strncpy(id,i,50);
      id[49]='\0';
    }
    bool operator<(const Player& other)const
    {
     return score>other.score;
    }
};


class Leaderboard 
{
private:
    Player heap[MAX_PLAYERS];
    int heapSize;

    int getParentIndex(int index) const 
    {
        return (index-1)/2;
    }

    int getLeftChildIndex(int index) const 
    {
        return 2*index+1;
    }

    int getRightChildIndex(int index) const 
    {
        return 2*index+2;
    }

    void swap(Player& a, Player& b) 
    {
        Player temp=a;
        a=b;
        b=temp;
    }

    void heapifyDown(int index) 
    {
        int left=getLeftChildIndex(index);
        int right=getRightChildIndex(index);
        int smallest=index;

        if (left<heapSize && heap[left].score<heap[smallest].score)
        {
            smallest=left;
        }    
        if (right<heapSize && heap[right].score<heap[smallest].score)
        {
            smallest=right;
        }    
        if (smallest != index) 
        {
            swap(heap[index],heap[smallest]);
            heapifyDown(smallest);
        }
    }

    void heapifyUp(int index) 
    {
        while (index>0 && heap[getParentIndex(index)].score>heap[index].score) 
        {
            swap(heap[index],heap[getParentIndex(index)]);
            index=getParentIndex(index);
        }
    }

    void loadFromFile() 
    {
        ifstream file("leaderboard.txt",ios::in);
        char id[50];
        int score;
        heapSize=0;

        while (file>>id>>score) 
        {
            insertIntoHeap(id,score);
        }
        file.close();
    }

    void saveToFile() 
    {
        ofstream file("leaderboard.txt",ios::out | ios::trunc);
        for (int i=0;i<heapSize;i++) 
        {
            file<<heap[i].id<<" "<<heap[i].score<<endl;
        }
        file.close();
    }

    void insertIntoHeap(const char* id, int score) 
    {
        Player newPlayer(id, score);
        if (heapSize<MAX_PLAYERS) 
        {
            heap[heapSize++]=newPlayer;
            heapifyUp(heapSize-1);
        } 
        else if (score>heap[0].score) 
        {
            heap[0]=newPlayer;
            heapifyDown(0);
        }
    }

public:
    Leaderboard():heapSize(0) 
    {
        loadFromFile();
    }

    void updatePlayerScore(const char* id, int newScore) 
   {
    bool found=false;
    for (int i=0;i<heapSize;i++) 
    {
        if (strcmp(heap[i].id,id)==0) 
        {
            // Player exists, check if new score is higher
            if (newScore>heap[i].score) 
            {
                heap[i].score=newScore;  // Update to the higher score
                heapifyUp(i);  // Reorder heap after score update
            }
            found=true;
            break;
        }
    }
    if (!found) 
    {
        // If player doesn't exist, add them to the leaderboard
        insertIntoHeap(id, newScore);
    }
    saveToFile();  // Save the updated leaderboard to the file
}

    
    void displayLeaderboard(sf::RenderWindow& window) 
    {
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("1.png")) 
    {
        cout<< "Failed to load background image"<<endl;
    }
    sf::Sprite background(backgroundTexture);

    sf::Font font;
    if (!font.loadFromFile("ByteBounce.ttf")) 
    {
        cout<< "Failed to load font."<<endl;
    }

    window.clear();
    window.draw(background);

    sf::Text title("Top 10 Players",font,70);
    title.setPosition(180,20);
    title.setFillColor(sf::Color::Cyan);
    window.draw(title);

    Player temp[MAX_PLAYERS];
    for (int i=0;i<heapSize;i++) 
    {
        temp[i]=heap[i];
    }

    for(int i=0;i<heapSize-1;i++)
    {
        for(int j=0;j<heapSize-i-1;j++) 
        {
            if (temp[j].score<temp[j+1].score) 
            {
                Player t=temp[j];
                temp[j]=temp[j+1];
                temp[j+1]=t;
            }
        }
    }

    for (int i=0;i<heapSize;i++) 
    {
        sf::Text line;
        line.setFont(font);
        line.setCharacterSize(40);
        line.setFillColor(sf::Color::White);
        line.setString(to_string(i+1) + ". " +temp[i].id+" - " +to_string(temp[i].score));
        line.setPosition(200,100+i*50);
        window.draw(line);
    }

    window.display();

    while (true) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) 
               {
                window.close();
                return;
               }
        }
    }
}

};



#endif

