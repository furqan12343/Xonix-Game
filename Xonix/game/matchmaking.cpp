#include<iostream>
#include<fstream>
#include<cstring>
using namespace std;


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

    PriorityQueue() 
    {
        front = NULL;
    }

    void enqueue(int id, int score, const char* name) 
    {
        PlayerNode* temp=new PlayerNode;
        temp->id=id;
        temp->score=score;
        strcpy(temp->name,name);
        temp->next=NULL;

        // If the queue is empty or this player has a higher score, insert at the front
        if (front == NULL||front->score < score) 
        {
            temp->next = front;
            front = temp;
        } 
        else 
        {
            PlayerNode* current=front;
            while (current->next!=NULL && current->next->score>=score) 
            {
                current=current->next;
            }
            temp->next=current->next;
            current->next=temp;
        }
        printQueue();
    }

    bool dequeue(int& id, int& score) 
    {
        if (front==NULL) 
        {
         return false;
        } 

        PlayerNode* temp=front;
        front=front->next;
        id=temp->id;
        score=temp->score;
        delete temp;

        printQueue();
        return true;
    }

    bool isEmpty() 
    {
        return front==NULL;
    }

    void printQueue() 
    {
        PlayerNode* current = front;
        cout<<"Current Queue: ";
        while (current != NULL) 
        {
            cout<<"("<<current->id<<", "<<current->score<<") ";
            current = current->next;
        }
        cout<<endl;
    }

    ~PriorityQueue() 
    {
        while (front!=NULL) 
        {
            PlayerNode* temp=front;
            front=front->next;
            delete temp;
        }
    }
};

void readUserName(int id, char name[]) 
{
    ifstream inFile("users.txt");
    char line[256];
    while(inFile.getline(line, 256)) 
    {
        int fileId;
        sscanf(line,"ID: %d | Username: %[^|]",&fileId,name);
        if(fileId==id) 
        {
            inFile.close();
            return;
        }
    }
    inFile.close();
    strcpy(name,"Unknown");
}

bool isAlreadyMatched(int id1, int id2) 
{
    ifstream file("matched.txt");
    int a,b;
    while (file>>a>> b) 
    {
        if ((a==id1 && b== id2) || (a==id2 && b==id1)) 
        {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

void saveMatchedPair(int id1, int id2) 
{
    ofstream file("matched.txt", ios::app);
    file<<id1<<" "<<id2<<endl;
    file.close();
}

void getTopTwoPlayers(int& top1, int& top2) 
{
    PriorityQueue pq;

    ifstream usr("users.txt");
    int userIds[100], userCount = 0;
    char line[200];

    while(usr.getline(line, 200)) 
    {
        int i=0;
        while(line[i] && !(line[i]>='0' && line[i]<='9')) 
        {
         i++;
        } 
        int id=0;
        while(line[i]>='0' && line[i]<='9') 
        {
            id=id*10+(line[i]-'0');
            i++;
        }
        userIds[userCount++]=id;
    }
    usr.close();

    ifstream ldr("leaderboard.txt");
    int id, score;
    int matchedCount=0;
    while(ldr>>id>>score) 
    {
        bool found=false;
        for(int i=0;i<userCount;i++) 
        {
            if(userIds[i]==id) 
            {
                found=true;
                break;
            }
        }
        if(found) 
        {
            char name[50];
            readUserName(id, name);
            pq.enqueue(id, score, name);
            matchedCount++;
        }
    }
    ldr.close();

    int p1_id, p1_score, p2_id, p2_score;
    bool gotFirst=pq.dequeue(p1_id, p1_score);
    bool gotSecond=pq.dequeue(p2_id, p2_score);

    while (gotFirst && gotSecond) 
    {
        if (!isAlreadyMatched(p1_id, p2_id)) 
        {
            top1=p1_id;
            top2=p2_id;
            saveMatchedPair(p1_id,p2_id);
            return;
        } 
        else 
        {
            // Try next pair
            gotFirst=gotSecond;
            p1_id=p2_id;
            gotSecond=pq.dequeue(p2_id, p2_score);
        }
    }

    top1=-1;
    top2=-1;
    cout<<"No unmatched player pairs available."<<endl;
}

