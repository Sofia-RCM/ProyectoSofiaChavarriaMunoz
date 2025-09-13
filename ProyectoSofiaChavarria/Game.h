#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Board.h"
using namespace sf;
extern std::map<std::string, sf::Texture> TEXTURES;

class Game {
private:
    int punctuation;
    int counter;
    Board board;

    bool gameOver = false;
    Font font;
    Text scoreText;
    Text movesText;
    Text overText;
    bool hudOk = false;

    // ?? Nuevo
    int state = 0; // 0 = MENU, 1 = PLAYING, 2 = GAME_OVER
    RectangleShape playButton;
    Text playText;

    void updateHUD();

public:
    Game();
    void run();
    bool loadTexture(const std::string& name, const std::string& path);
};
