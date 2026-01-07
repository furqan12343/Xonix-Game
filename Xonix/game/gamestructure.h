// GameStructure.h
#pragma once
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

const int m = 25;
const int n = 40;

struct TileNode 
{
    int row, col, type;
    TileNode* next;
    TileNode(int r, int c, int t) : row(r), col(c), type(t), next(nullptr) 
    {}
};

struct GameStructure 
{
    string saveID;
    string username;
    time_t timestamp;
    int grid[m][n];
    int x, y, dx, dy;
    int score;
    int rewardCount;
    int capturedTiles;
    int powerUpInventory;
    bool powerUpActive;
    float powerUpTimer;

    // Linked list of tiles interacted with
    TileNode* tileHead = nullptr;

    // Update tile list when interacting with a tile
    void updateTile(int row, int col, int type) 
    {
        TileNode* temp = tileHead;
        while (temp) 
        {
            if (temp->row == row && temp->col == col) 
            {
                temp->type = type;
                return;
            }
            temp = temp->next;
        }
        TileNode* newNode = new TileNode(row, col, type);
        newNode->next = tileHead;
        tileHead = newNode;
    }

    // Use this function instead of grid[row][col] = type
    void setTile(int row, int col, int type) 
    {
        if (row >= 0 && row < m && col >= 0 && col < n) 
        {
            grid[row][col] = type;
            updateTile(row, col, type);
        }
    }

    // Clear the linked list memory
    void clearTileList() 
    {
        TileNode* current = tileHead;
        while (current) 
        {
            TileNode* next = current->next;
            delete current;
            current = next;
        }
        tileHead = nullptr;
    }

    // Save game to file
    void saveToFile() 
    {
        ofstream out("saves/" + saveID + ".txt");
        if (!out) 
        {
            cout<<"Error creating save file.\n";
            return;
        }
        out<<timestamp<<endl;
        out<<x<<" "<<y<<" "<<dx<<" "<<dy<<endl;
        out<<score<<" "<<rewardCount<<" "<<capturedTiles<<endl;
        out<<powerUpInventory<<" "<<powerUpActive<<" "<<powerUpTimer<<endl;
        for (int i=0;i<m;i++)
            for (int j=0;j<n;j++)
                out<<grid[i][j]<<" ";
        out<<endl;

        // Save tile list
        TileNode* temp = tileHead;
        while (temp) 
        {
            out<<temp->row<<" "<<temp->col<<" "<<temp->type<<endl;
            temp = temp->next;
        }
        out<<-1<<" "<<-1<<" "<<-1<<endl; // Terminator
        out.close();
        cout<<"Game saved with ID: "<<saveID<<endl;
    }

    // Load game from file
    bool loadFromFile(const string& id) 
    {
        ifstream in("saves/" + id + ".txt");
        if (!in) 
        {
            cout<<"Save file not found."<<endl;
            return false;
        }
        saveID = id;
        in>>timestamp;
        in>>x>>y>>dx>>dy;
        in>>score>>rewardCount>>capturedTiles;
        in>>powerUpInventory>>powerUpActive>>powerUpTimer;
        for (int i=0;i<m;i++)
            for (int j=0;j<n;j++)
                in>>grid[i][j];

        clearTileList(); // Ensure no memory leaks
        while (true) 
        {
            int r, c, t;
            in>>r>>c>>t;
            if (r == -1 && c == -1 && t == -1) break;
            updateTile(r, c, t);
        }
        in.close();
        return true;
    }
};

