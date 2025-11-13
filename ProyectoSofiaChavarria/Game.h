#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Board.h"
#include "Ranking.h"
#include "ProgressManager.h"

#include "Ranking.h"
enum class GameState {
    Menu, Playing, Swapping, Clearing, Falling, GameOver, WinAll
};

class Game {

using namespace sf;
extern map<string, Texture> TEXTURES;

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

    int state = 0;
    RectangleShape playButton;
    Text playText;

    Texture bgTexture;
    Sprite bgSprite;

    void updateHUD();


public:
    Game();
    ~Game();
    void run();

private:
<<<<<<< HEAD
    bool nivel1Unlocked = true;
    bool nivel2Unlocked = false;
    bool nivel3Unlocked = false;

    bool nivel1Completado = false;
    bool nivel2Completado = false;
    bool nivel3Completado = false;

    void loadProgress();
    void saveProgress();


    Ranking ranking;
    // --- Estado general ---
    int state = 0;
=======
    // ---- Progreso por jugador ----
    ProgressManager progress;       // lee/escribe progress.txt
    std::string playerName;         // nombre actual

    // ---- Estado general ----
    int state = 0; // 0 menú principal, 1 jugando, 2 final, 3 menú de niveles
>>>>>>> develop
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
<<<<<<< HEAD
    // --- Niveles ---
=======

    // ---- Niveles y objetivos ----
>>>>>>> develop
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

    bool loadTexture(const string& name, const string& path);
};

