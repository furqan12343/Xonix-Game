// friend_system.h
#ifndef FRIEND_SYSTEM_H
#define FRIEND_SYSTEM_H

#include <SFML/Graphics.hpp>
#include <string>

const int _max= 100;

struct FriendNode
{
    std::string friend_username;
    FriendNode *next;
};

struct player
{
    std::string username;
    FriendNode *friend_head;
};

extern player players[_max];
extern int hash_table[_max];
extern int total_players;

void load_players();
int find_player_index(const std::string username);
bool already_friends(const std::string &user1, const std::string user2);
void add_friend_to_file(const std::string &user1, const std::string user2);
void send_friend_request(const std::string &sender, const std::string receiver);
void show_friend_menu(sf::RenderWindow &window, const std::string current_user);
void display_text(sf::RenderWindow &window, sf::Font &font, const std::string msg, int x, int y, sf::Color color, int size);

#endif

