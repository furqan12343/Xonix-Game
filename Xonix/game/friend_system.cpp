/*
#include <SFML/Graphics.hpp>

#include "friend_system.h"
#include <fstream>
#include <iostream>
#include <cstring>
using namespace sf;
using namespace std;
player players[_max];
int hash_table[_max];
int total_players = 0;

void load_players()
{
    ifstream file("users.txt");
    string line;
    total_players = 0;

    while (getline(file, line) && total_players < _max)
    {
        size_t u_start = line.find("Username: ");
        if (u_start != string::npos)
        {
            u_start += 10;
            size_t u_end = line.find(" |", u_start);
            string uname = line.substr(u_start, u_end - u_start);
            players[total_players].username = uname;
            players[total_players].friend_head = NULL;
            hash_table[total_players] = total_players;
            total_players++;
        }
    }
    file.close();
}

// (The rest of your original code remains unchanged...)


int find_player_index(const string username)
{
    for (int i = 0; i < total_players; i++)
    {
        if (players[i].username == username)
            return i;
    }
    return -1;
}

bool already_friends(const string &user1, const string user2)
{
    string file_path = "profiles/" + user1 + ".txt";
    ifstream profile(file_path.c_str());
    string line;
    while (getline(profile, line))
    {
        if (line.find("Friend:" + user2) == 0)
            return true;
    }
    return false;
}

void add_friend_to_file(const string &user1, const string user2)
{
    string file_path = "profiles/" + user1 + ".txt";
    ifstream infile(file_path.c_str());
    string lines[50];
    int count = 0;
    string line;

    while (getline(infile, line))
    {
        lines[count++] = line;
    }
    infile.close();

    lines[count++] = "Friend:" + user2;

    ofstream outfile(file_path.c_str());
    for (int i = 0; i < count; i++)
    {
        outfile << lines[i] << "\n";
    }
    outfile.close();
}

void send_friend_request(const string &sender, const string receiver)
{
    ofstream req_file("requests.txt", ios::app);
    req_file << sender << "," << receiver << endl;
    req_file.close();
}

void display_text(RenderWindow &window, Font &font, const string msg, int x, int y, Color color, int size = 24)
{
    Text text;
    text.setFont(font);
    text.setString(msg);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(x, y);
    window.draw(text);
}

void show_friend_menu(RenderWindow &window, const string current_user)
{
    load_players();

    Font font;
    if (!font.loadFromFile("arial.ttf"))
        return;

    int selected = 0;
    string menu[3] = {"Send Friend Request", "View Pending Requests", "Back"};

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
                    selected = (selected + 2) % 3;
                if (e.key.code == Keyboard::Down)
                    selected = (selected + 1) % 3;
                if (e.key.code == Keyboard::Enter)
                {
                    if (selected == 0)
                    {
                        // Send Friend Request screen
                        int target_index = 0;
                        while (window.isOpen())
                        {
                            Event f;
                            while (window.pollEvent(f))
                            {
                                if (f.type == Event::Closed)
                                    window.close();
                                if (f.type == Event::KeyPressed)
                                {
                                    if (f.key.code == Keyboard::Up)
                                        target_index = (target_index + total_players - 1) % total_players;
                                    if (f.key.code == Keyboard::Down)
                                        target_index = (target_index + 1) % total_players;
                                    if (f.key.code == Keyboard::Enter)
                                    {
                                        string target = players[target_index].username;
                                        if (target != current_user && !already_friends(current_user, target))
                                        {
                                            send_friend_request(current_user, target);
                                        }
                                        return;
                                    }
                                    if (f.key.code == Keyboard::Escape)
                                        return;
                                }
                            }
                            window.clear();
                            display_text(window, font, "Send Friend Request:", 100, 30, Color::White, 28);
                            for (int i = 0; i < total_players; i++)
                            {
                                if (players[i].username == current_user) continue;
                                Color c = (i == target_index ? Color::Green : Color::White);
                                display_text(window, font, players[i].username, 100, 80 + i * 30, c);
                            }
                            window.display();
                        }
                    }
                    else if (selected == 1)
                    {
                        // View and Accept/Reject Requests
                        ifstream req_file("requests.txt");
                        string requests[10];
                        int req_count = 0;
                        string line;
                        while (getline(req_file, line) && req_count < 10)
			{
			    size_t first_comma = line.find(',');
			    size_t second_space = line.find(' ', first_comma + 1); // space after the recipient

			    string from = line.substr(0, first_comma);
			    string to = line.substr(first_comma + 1, second_space - first_comma - 1);

			    if (to == current_user)
				requests[req_count++] = from;
			}
                        req_file.close();

                        int req_index = 0;
                        while (window.isOpen())
                        {
                            Event r;
                            while (window.pollEvent(r))
                            {
                                if (r.type == Event::Closed)
                                    window.close();
                                if (r.type == Event::KeyPressed)
                                {
                                    if (r.key.code == Keyboard::Up)
                                        req_index = (req_index + req_count - 1) % req_count;
                                    if (r.key.code == Keyboard::Down)
                                        req_index = (req_index + 1) % req_count;
                                    if (r.key.code == Keyboard::A)
                                    {
                                        string sender = requests[req_index];
                                        add_friend_to_file(current_user, sender);
                                        add_friend_to_file(sender, current_user);
                                        // Remove request
                                        ifstream in("requests.txt");
                                        ofstream temp("temp.txt");
                                        string l;
                                        while (getline(in, l))
                                        {
                                            if (l != sender + "," + current_user)
                                                temp << l << "\n";
                                        }
                                        in.close();
                                        temp.close();
                                        remove("requests.txt");
                                        rename("temp.txt", "requests.txt");
                                        return;
                                    }
                                    if (r.key.code == Keyboard::Escape)
                                        return;
                                }
                            }
                            window.clear();
                            display_text(window, font, "Pending Requests (A to Accept):", 100, 30, Color::White, 26);
                            for (int i = 0; i < req_count; i++)
                            {
                                Color c = (i == req_index ? Color::Green : Color::White);
                                display_text(window, font, requests[i], 100, 80 + i * 30, c);
                            }
                            window.display();
                        }
                    }
                    else if (selected == 2)
                    {
                        return;
                    }
                }
            }
        }

        window.clear();
        display_text(window, font, "Friend Menu", 150, 30, Color::White, 32);
        for (int i = 0; i < 3; i++)
        {
            Color c = (i == selected ? Color::Cyan : Color::White);
            display_text(window, font, menu[i], 150, 100 + i * 40, c);
        }
        window.display();
    }
}*/
