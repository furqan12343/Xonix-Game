#ifndef REGISTER_H
#define REGISTER_H

#include<SFML/Graphics.hpp>
#include<string>

using namespace std;
using namespace sf;

bool usernameExists(const string& username);
bool isUpper(char ch);
bool isLower(char ch);
bool isDigit(char ch);
bool isSpecial(char ch);
bool isStrongPassword(const string& password);
string getTimestamp();
int getNextID();
bool registerpage();

#endif 

