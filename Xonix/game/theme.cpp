#include "theme.h"
#include <iostream>
#include <cstdlib>
using namespace std;

int max(int a,int b) 
{
    return (a>b)? a : b;
}

// ---------- Theme Class ----------

Theme::Theme()
{
    id=0;
    name="";
    description="";
    color=sf::Color::White;
}


Theme::Theme(int id,const string& name,const string& desc,const sf::Color& color)
    :id(id),name(name),description(desc),color(color) {}

void Theme::display()const 
{
    cout<<"ID:"<<id<<"\nName:"<<name<<"\nDescription:"<<description
        <<"\nColor(RGB):("<<(int)color.r<<","<<(int)color.g<<","<<(int)color.b<<")"<<endl;
}

void Theme::getRGBCodes(int &r,int &g,int &b)const 
{
    r=color.r;
    g=color.g;
    b=color.b;
}



// ---------- AVLNode ----------

AVLNode::AVLNode(const Theme& theme)
{
    data=theme;
    left=right=nullptr;
    height=1;
}


// ---------- ThemeInventory ----------

ThemeInventory::ThemeInventory() : root(nullptr), numThemes(0) {}

int ThemeInventory::height(AVLNode* node) 
{
    if (!node) return 0;
    return node->height;
}

int ThemeInventory::getBalance(AVLNode* node) 
{
    if (!node) return 0;
    return height(node->left) - height(node->right);
}

AVLNode* ThemeInventory::rotateLeft(AVLNode* x) 
{
    AVLNode* y = x->right;
    x->right = y->left;
    y->left = x;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

AVLNode* ThemeInventory::rotateRight(AVLNode* y) 
{
    AVLNode* x = y->left;
    y->left = x->right;
    x->right = y;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

AVLNode* ThemeInventory::insert(AVLNode* node, Theme theme) 
{
    if (!node) return new AVLNode(theme);

    if (theme.getId() < node->data.getId())
        node->left = insert(node->left, theme);
    else if (theme.getId() > node->data.getId())
        node->right = insert(node->right, theme);
    else
        return node;

    node->height = max(height(node->left), height(node->right)) + 1;
    int balance = getBalance(node);

    if (balance > 1 && theme.getId() < node->left->data.getId())
        return rotateRight(node);
    if (balance < -1 && theme.getId() > node->right->data.getId())
        return rotateLeft(node);
    if (balance > 1 && theme.getId() > node->left->data.getId()) 
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && theme.getId() < node->right->data.getId()) 
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

void ThemeInventory::insertTheme(Theme theme) 
{
    root = insert(root, theme);
}

void ThemeInventory::inOrderTraversal(AVLNode* node) 
{
    if (!node) return;
    inOrderTraversal(node->left);
    node->data.display();
    inOrderTraversal(node->right);
}

int ThemeInventory::collectThemes(AVLNode* node, Theme themes[], int index) 
{
    if (!node) return index;
    index = collectThemes(node->left, themes, index);
    themes[index++] = node->data;
    index = collectThemes(node->right, themes, index);
    return index;
}

int ThemeInventory::getThemes(Theme themes[]) 
{
    return collectThemes(root, themes, 0);
}


void ThemeInventory::displayThemes() 
{
    inOrderTraversal(root);
}

AVLNode* ThemeInventory::searchById(AVLNode* node, int id) 
{
    if (!node) return nullptr;
    if (id == node->data.getId()) return node;
    else if (id < node->data.getId()) return searchById(node->left, id);
    else return searchById(node->right, id);
}

Theme* ThemeInventory::searchThemeById(int id) 
{
    AVLNode* result = searchById(root, id);
    if (result) return &result->data;
    return nullptr;
}

void ThemeInventory::loadThemesFromFile(const string& filename) 
{
    ifstream file(filename.c_str());
    if (!file.is_open()) return;

    char line[256];
    while (file.getline(line, 256)) 
    {
        int id = 0, r = 0, g = 0, b = 0;
        char name[50], desc[100];
        int i = 0, j = 0, field = 0;
        char temp[6][100] = {{0}};

        while (line[i] != '\0') 
        {
            if (line[i] == ',') 
            {
                temp[field][j] = '\0';
                field++;
                j = 0;
            } 
            else 
            {
                temp[field][j++] = line[i];
            }
            i++;
        }
        temp[field][j] = '\0';

        id = atoi(temp[0]);
        for (i = 0; i < 50; i++) name[i] = temp[1][i];
        for (i = 0; i < 100; i++) desc[i] = temp[2][i];
        r = atoi(temp[3]);
        g = atoi(temp[4]);
        b = atoi(temp[5]);

        Theme theme(id, string(name), string(desc), sf::Color(r, g, b));
        insertTheme(theme);
    }

    file.close();
}


