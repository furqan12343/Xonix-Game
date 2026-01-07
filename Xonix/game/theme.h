#ifndef THEME_H
#define THEME_H

#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>

#define MAX_THEMES 10


class Theme 
{
public:
    Theme();
    Theme(int id, const std::string& name, const std::string& desc, const sf::Color& color);
    void display() const;
    int getId() const { return id; }
    std::string getName() const { return name; }
    void getRGBCodes(int &r, int &g, int &b) const;
    std::string getDescription() const { return description; }
    sf::Color getColor() const { return color; }

private:
    int id;
    std::string name;
    std::string description;
    sf::Color color;
};

class AVLNode 
{
public:
    Theme data;
    AVLNode* left;
    AVLNode* right;
    int height;
    AVLNode(const Theme& theme);
};

class ThemeInventory 
{
public:
    ThemeInventory();
    void insertTheme(Theme theme);
    void displayThemes();
    Theme* searchThemeById(int id);
    void loadThemesFromFile(const std::string& filename);
    int getThemes(Theme themes[]);
    int collectThemes(AVLNode* node, Theme themes[], int index);
    
private:
    AVLNode* root;
    int numThemes;
    
    int height(AVLNode* node);
    int getBalance(AVLNode* node);
    AVLNode* rotateLeft(AVLNode* x);
    AVLNode* rotateRight(AVLNode* y);
    AVLNode* insert(AVLNode* node, Theme theme);
    AVLNode* searchById(AVLNode* node, int id);
    void inOrderTraversal(AVLNode* node);
    int compareStrings(std::string a, std::string b);
};




#endif // THEME_H

