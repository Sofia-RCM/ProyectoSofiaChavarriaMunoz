#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Board.h"

enum class GameState {
    Menu, Playing, Swapping, Clearing, Falling, GameOver, WinAll
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
    sf::Text scoreText, movesText, overText, levelText, objectiveText; // ? nuevo

    sf::Texture bgTexture, bgFinalTexture;
    sf::Sprite  bgSprite, bgFinalSprite;

    sf::RectangleShape playButton, restartButton, exitButton;
    sf::Text playText, restartText, exitText;

    int punctuation = 0;
    int counter = 20;

    Board board;

    // Objetivos por nivel
    int totoroCleared = 0;
    int iceCleared = 0;
    int ponyoCleared = 0;

    const int TOTORO_GOAL = 30;
    const int ICE_GOAL = 5;
    const int PONYO_GOAL = 50;

    int currentLevel = 1;

    int movesSinceIce = 0;
    static const int ICE_EVERY = 5;

    int selR = -1, selC = -1;

    void updateHUD();
    static void coverSpriteToWindow(sf::Sprite& spr, const sf::Vector2u& win);

    void startGame();
    void nextLevel();
    void checkLevelAdvance();
    void processCascadesOnce();
};