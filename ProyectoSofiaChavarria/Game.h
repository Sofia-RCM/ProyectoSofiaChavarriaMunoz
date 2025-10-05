#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Board.h"
#include "HUD.h"
#include "ResourceManager.h"
#include "Constants.h"
using namespace sf;
using namespace std;

class Game {
private:
    int punctuation;
    int counter;
    Board board;
    HUD* hud;
    bool gameOver = false;
    int state = 0;

    Texture bgTexture;
    Sprite bgSprite;
    Texture bgFinalTexture;
    Sprite bgFinalSprite;

    RectangleShape playButton;
    Text playText;
    RectangleShape restartButton;
    Text restartText;
    RectangleShape exitButton;
    Text exitText;

public:
    Game();
    void run();
};
