#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include "Board.h"
using namespace sf;
using namespace std;

extern map<string, Texture> TEXTURES;

class Game {
private:
    int punctuation;
    int counter;
    Board board;

    bool gameOver = false;
    bool hudOk = false;
    int state = 0;

    Font font;
    Text scoreText;
    Text movesText;
    Text overText;

    RectangleShape playButton;
    Text playText;
    Texture bgTexture;
    Sprite bgSprite;

    RectangleShape restartButton;
    Text restartText;
    RectangleShape exitButton;
    Text exitText;
    Texture bgFinalTexture;
    Sprite bgFinalSprite;

    void updateHUD();

public:
    Game();
    void run();
    bool loadTexture(const string& name, const string& path);
};
