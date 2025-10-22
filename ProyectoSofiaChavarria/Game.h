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
    int state = 0;                // 0=menu,1=juego,2=final
    GameState gameState = GameState::Menu;
    bool gameOver = false;

    sf::Font font;
    bool hudOk = false;
    sf::Text scoreText, movesText, overText, levelText;

    sf::Texture bgTexture, bgFinalTexture;
    sf::Sprite  bgSprite, bgFinalSprite;

    sf::RectangleShape playButton, restartButton, exitButton;
    sf::Text playText, restartText, exitText;

    int punctuation = 0;
    int counter = 20;

    Board board;

    // Niveles
    int currentLevel = 1;
    const int levelGoals[3] = { 200, 400, 600 };

    // Cascadas temporizadas
    float stateTimer = 0.f;

    // Ice cada X movimientos
    int movesSinceIce = 0;
    static const int ICE_EVERY = 5; // confirmado por ti

    // selección
    int selR = -1, selC = -1;

    void updateHUD();
    static void coverSpriteToWindow(sf::Sprite& spr, const sf::Vector2u& win);

    void startGame();
    void nextLevel();
    void checkLevelAdvance();
    void processCascadesOnce();
};
