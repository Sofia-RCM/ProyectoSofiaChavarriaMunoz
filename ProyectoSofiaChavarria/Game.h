#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Board.h"

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
    const int TOTORO_GOAL = 30;
    const int ICE_GOAL = 10;
    const int PONYO_GOAL = 20;

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
    void nextLevel();
    void checkLevelAdvance();
    void processCascadesOnce();

    // --- Animación de transición de nivel ---
    void showLevelTransition(sf::RenderWindow& window, int nextLevel);
};

    bool loadTexture(const string& name, const string& path);
};

