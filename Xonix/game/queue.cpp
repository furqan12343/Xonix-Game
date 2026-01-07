#include<iostream>
#include<fstream>
#include<cstring>
#include "matchmaking.h"
using namespace std;

// PriorityQueue constructor
PriorityQueue::PriorityQueue() {
    front = NULL;
}

// enqueue method for PriorityQueue
void PriorityQueue::enqueue(int id, int score, const char* name) {
    PlayerNode* temp = new PlayerNode;
    temp->id = id;
    temp->score = score;
    strcpy(temp->name, name);
    temp->next = NULL;

    // Insert at the right position based on score
    if(front == NULL || front->score < score) {
        temp->next = front;
        front = temp;
    } else {
        PlayerNode* current = front;
        while(current->next != NULL && current->next->score >= score) {
            current = current->next;
        }
        temp->next = current->next;
        current->next = temp;
    }
}

// dequeue method for PriorityQueue
bool PriorityQueue::dequeue(int& id, int& score) {
    if(front == NULL) return false;

    PlayerNode* temp = front;
    front = front->next;
    id = temp->id;
    score = temp->score;
    delete temp;
    return true;
}

// Checking if PriorityQueue is empty
bool PriorityQueue::isEmpty() {
    return front == NULL;
}

// Destructor for PriorityQueue
PriorityQueue::~PriorityQueue() {
    while(front != NULL) {
        PlayerNode* temp = front;
        front = front->next;
        delete temp;
    }
}

// Function to load previously used players from a file
void loadUsedPlayers(int usedPlayers[], int& usedCount) {
    ifstream inFile("used_players.txt");
    usedCount = 0;
    if(inFile.is_open()) {
        while(inFile >> usedPlayers[usedCount]) {
            usedCount++;
        }
        inFile.close();
    }
}

// Checking if a player has already been used
bool isUsed(int id, int usedPlayers[], int usedCount) {
    for(int i = 0; i < usedCount; i++) {
        if(usedPlayers[i] == id)
            return true;
    }
    return false;
}

// Mark a pair of players as used
void markAsUsed(int id1, int id2) {
    ofstream outFile("used_players.txt", ios::app);
    if(outFile.is_open()) {
        outFile << id1 << " " << id2 << endl;
        outFile.close();
    }
}

// Function to read the username from the file based on ID
void readUserName(int id, char name[]) {
    ifstream inFile("users.txt");
    char line[256];
    while(inFile.getline(line, 256)) {
        int fileId;
        sscanf(line, "ID: %d | Username: %[^|]", &fileId, name);
        if(fileId == id) {
            inFile.close();
            return;
        }
    }
    inFile.close();
    strcpy(name, "Unknown");
}

// Function to read the leaderboard and populate the PriorityQueue
void readLeaderboard(PriorityQueue& pq, int usedPlayers[], int usedCount) {
    ifstream inFile("leaderboard.txt");
    int id, score;
    while(inFile >> id >> score) {
        if(!isUsed(id, usedPlayers, usedCount)) {
            char name[50];
            readUserName(id, name);
            pq.enqueue(id, score, name);
        }
    }
    inFile.close();
}

// Function to get the top two players from the leaderboard
void getTopTwoPlayers(int& top1, int& top2) {
    PriorityQueue pq;

    ifstream ldr("leaderboard.txt");
    ifstream usr("users.txt");

    int id, score;
    char line[100];

    // Read leaderboard and process players
    while(ldr >> id >> score) {
        PlayerNode* p = new PlayerNode; // Use PlayerNode instead of Player
        p->id = id;
        p->score = score;
        p->next = NULL;
        p->name[0] = '\0';

        // Read username from users.txt
        usr.clear();
        usr.seekg(0);
        while(usr.getline(line, 100)) {
            int uid;
            char uname[50];
            sscanf(line, "ID: %d | Username: %[^|]", &uid, uname);
            if(uid == id) {
                strcpy(p->name, uname);
                break;
            }
        }

        pq.enqueue(p->id, p->score, p->name); // Correct function call here
    }

    usr.close();
    ldr.close();

    int p1_id, p1_score, p2_id, p2_score;
    pq.dequeue(p1_id, p1_score); // Get top 1
    pq.dequeue(p2_id, p2_score); // Get top 2

    if(p1_id != -1 && p2_id != -1) {
        top1 = p1_id;
        top2 = p2_id;

        ofstream fout("used_players.txt", ios::app);
        fout << top1 << endl;
        fout << top2 << endl;
        fout.close();
    } else {
        top1 = -1;
        top2 = -1;
    }
}

