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

    Texture texture;
    Sprite  sprite;
    bool loaded = false;
    string imagePath;

    RectangleShape cross1;
    RectangleShape cross2;

    void applyScaleAndPosition();

public:
    Gem();
    void setTipoGem(string& tipo);
    string getTipoGem();

    string getImage();
    bool   isLoaded();

    void setGrid(int r, int c, int cell);
    void setImage(string path, int cell);

    void draw(sf::RenderWindow& window);
    bool isEmpty();
};
