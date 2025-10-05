#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
using namespace std;
using namespace sf;

class Board;

class Gem {
protected:
    string type;
    int row = 0;
    int col = 0;
    int cellSize = 64;
    Sprite sprite;
    bool loaded = false;

public:
    virtual ~Gem() {}
    virtual void setGrid(int r, int c, int cell);
    virtual void setPosition(float cx, float cy);
    virtual void draw(RenderWindow& window) = 0;
    virtual void onMatch(Board& board) {}
    virtual string getType() const = 0;
    virtual bool isSpecial() const { return false; }
    bool isEmpty() const { return type.empty(); }
    bool isLoaded() const { return loaded; }

protected:
    void applyScaleAndPosition();
};
