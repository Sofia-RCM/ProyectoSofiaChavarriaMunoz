#pragma once
#include <string>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

class Gem {
private:
    string tipoGem;
    int row = 0;
    int col = 0;
    int cellSize = 64;

    Sprite  sprite;
    bool loaded = false;

    RectangleShape cross1;
    RectangleShape cross2;

    void applyScaleAndPosition();

public:
    Gem();
    virtual ~Gem();  // ?? AÑADE ESTO ?? Destructor virtual

    void setTipoGem(const string& tipo);
    string getTipoGem();

    bool   isLoaded();

    void setGrid(int r, int c, int cell);
    void setPosition(float cx, float cy);
    void draw(sf::RenderWindow& window);
    bool isEmpty();
};
