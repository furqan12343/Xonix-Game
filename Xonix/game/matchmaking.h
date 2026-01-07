#ifndef MATCHMAKING_H
#define MATCHMAKING_H

struct PlayerNode 
{
    int id;
    int score;
    char name[50];
    PlayerNode* next;
};

class PriorityQueue 
{
public:
    PlayerNode* front;
    PriorityQueue();
    void enqueue(int id, int score, const char* name);
    bool dequeue(int& id, int& score);
    bool isEmpty();
    PlayerNode* getFront() const 
    {
        return front;
    }
    ~PriorityQueue();
};

void loadUsedPlayers(int usedPlayers[], int& usedCount);
bool isUsed(int id, int usedPlayers[], int usedCount);
void markAsUsed(int id1, int id2);
void readLeaderboard(PriorityQueue& pq, int usedPlayers[], int usedCount);
void readUserName(int id, char name[]);
void getTopTwoPlayers(int& top1, int& top2);

#endif

