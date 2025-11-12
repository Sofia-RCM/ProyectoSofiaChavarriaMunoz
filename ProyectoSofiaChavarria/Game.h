#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Board.h"
#include "Ranking.h"


enum class GameState {
    Menu, Playing, Swapping, Clearing, Falling, GameOver, WinAll
};

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    // --- Progreso de niveles ---
    bool nivel1Unlocked = true;
    bool nivel2Unlocked = false;
    bool nivel3Unlocked = false;
    bool nivel1Completado = false;
    bool nivel2Completado = false;
    bool nivel3Completado = false;

    // --- Funciones para guardar/cargar progreso ---
    void loadProgress();
    void saveProgress();

    Ranking ranking;
    void showLevelTransition(sf::RenderWindow& window, int nextLevel);


    // --- Estado general ---
    int state = 0;
    GameState gameState = GameState::Menu;
    bool gameOver = false;

    // --- Fuente y HUD ---
    sf::Font font;
    bool hudOk = false;
    sf::Text scoreText, movesText, overText, levelText, objectiveText;

    // --- Fondo y texturas ---
    sf::Texture bgTexture, bgFinalTexture;
    sf::Sprite bgSprite, bgFinalSprite;

    // --- Botones ---
    sf::RectangleShape playButton, restartButton, exitButton;
    sf::Text playText, restartText, exitText;

    // --- Variables de juego ---
    int punctuation = 0;
    int counter = 20;

    Board board;

    // --- Niveles ---
    int currentLevel = 1;
    const int TOTORO_GOAL = 20;
    const int ICE_GOAL = 10;
    const int PONYO_GOAL = 30;

    int totoroCleared = 0;
    int iceCleared = 0;
    int ponyoCleared = 0;

    // --- Obstáculos dinámicos ---
    int movesSinceIce = 0;
    static const int ICE_EVERY = 5;

    // --- Selección ---
    int selR = -1, selC = -1;

    // --- NUEVO: esperando highlight especial ---
    bool waitingSpecial = false;

    // --- Métodos internos ---
    void updateHUD();
    static void coverSpriteToWindow(sf::Sprite& spr, const sf::Vector2u& win);
    void startGame();
    void nextLevel(sf::RenderWindow& window);
    void checkLevelAdvance(sf::RenderWindow& window);
    void processCascadesOnce(sf::RenderWindow& window);



};
