#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"

enum class GameState {
    Menu, Playing, Swapping, Clearing, Falling, GameOver
};

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    int state = 0;
    GameState gameState = GameState::Menu;
    bool gameOver = false;

    sf::Font font;
    bool hudOk = false;
    sf::Text scoreText, movesText, overText;

    sf::Texture bgTexture, bgFinalTexture;
    sf::Sprite bgSprite, bgFinalSprite;

    sf::RectangleShape playButton, restartButton, exitButton;
    sf::Text playText, restartText, exitText;

    int punctuation = 0;
    int counter = 20;
    Board board;

    float stateTimer = 0.f;
    const float CLEAR_DELAY = 0.3f;
    const float FALL_DELAY = 0.2f;

    int selR = -1, selC = -1;

    void updateHUD();
    static void coverSpriteToWindow(sf::Sprite& spr, const sf::Vector2u& win);
};