#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
using namespace std;
using namespace sf;

extern map<string, Texture> TEXTURES;

class Gem {
protected:
    string tipoGem;
    int row = 0;
    int col = 0;
    int cellSize = 64;
    bool loaded = false;
    Sprite sprite;

    void applyScaleAndPosition();

public:
    Gem() = default;
    virtual ~Gem() = default;

    virtual void setTipoGem(string& tipo);
    virtual string getTipoGem();
    void setGrid(int r, int c, int cell);
    virtual void setPosition(float cx, float cy);
    virtual void draw(RenderWindow& window);
    virtual bool isEmpty();
    virtual bool isLoaded();
    virtual void onMatch() = 0;
    virtual string getType() const = 0;
    int getRow() const { return row; }
    int getCol() const { return col; }
};
