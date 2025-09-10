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

    void updateHUD();

public:
    Game();
    void run();
    bool loadTexture(const std::string& name, const std::string& path);

};
