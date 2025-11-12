#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Board.h"
#include "Ranking.h"
#include "ProgressManager.h"

enum class GameState {
    Menu, Playing, Swapping, Clearing, Falling, GameOver, WinAll
};

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    // ---- Progreso por jugador ----
    ProgressManager progress;       // lee/escribe progress.txt
    std::string playerName;         // nombre actual

    // ---- Estado general ----
    int state = 0; // 0 menú principal, 1 jugando, 2 final, 3 menú de niveles
    GameState gameState = GameState::Menu;
    bool gameOver = false;

    // ---- Fuente y HUD ----
    sf::Font font;
    bool hudOk = false;
    sf::Text scoreText, movesText, overText, levelText, objectiveText;

    // ---- Fondo y texturas ----
    sf::Texture bgTexture, bgFinalTexture;
    sf::Sprite bgSprite, bgFinalSprite;

    // ---- Botones ----
    sf::RectangleShape playButton, restartButton, exitButton, menuButton;
    sf::Text playText, restartText, exitText, menuText;

    // ---- Variables de juego ----
    int punctuation = 0;
    int counter = 20;

    Board board;

    // ---- Niveles y objetivos ----
    int currentLevel = 1;
    const int TOTORO_GOAL = 20; // pedido: 20
    const int ICE_GOAL = 10;
    const int PONYO_GOAL = 30; // pedido: 30

    int totoroCleared = 0;
    int iceCleared = 0;
    int ponyoCleared = 0;

    // ---- Obstáculos dinámicos ----
    int movesSinceIce = 0;
    static const int ICE_EVERY = 5;

    // ---- Selección ----
    int selR = -1, selC = -1;

    // ---- Especiales (highlight) ----
    bool waitingSpecial = false;

    // ---- Menú de niveles (por jugador) ----
    bool nivel1Unlocked = true;
    bool nivel2Unlocked = false;
    bool nivel3Unlocked = false;

    // ---- Ranking ----
    Ranking ranking;

    // ---- Métodos internos ----
    void updateHUD();
    static void coverSpriteToWindow(sf::Sprite& spr, const sf::Vector2u& win);
    void startGame();                                // reinicia variables comunes
    void nextLevel(sf::RenderWindow& window);        // transición cremita + carga
    void checkLevelAdvance(sf::RenderWindow& window);
    void processCascadesOnce(sf::RenderWindow& window);
    void drawFinalScreen(sf::RenderWindow& window);

    // Pantalla de transición "Nivel X"
    void showLevelTransition(sf::RenderWindow& window, int nextLevel);
};
