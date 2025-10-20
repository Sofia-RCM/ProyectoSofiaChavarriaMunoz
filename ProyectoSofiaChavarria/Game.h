#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"

enum class GameState {
    Menu,
    Playing,
    Swapping,
    Clearing,
    Falling,
    GameOver
};

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    // Estado del juego
    int state = 0; // 0: menu, 1: juego, 2: final
    GameState gameState = GameState::Menu;
    bool gameOver = false;

    // HUD
    sf::Font font;
    bool hudOk = false;
    sf::Text scoreText, movesText, overText;

    // Fondos
    sf::Texture bgTexture, bgFinalTexture;
    sf::Sprite bgSprite, bgFinalSprite;

    // Botonera
    sf::RectangleShape playButton, restartButton, exitButton;
    sf::Text playText, restartText, exitText;

    // Juego
    int punctuation = 0;
    int counter = 20;
    Board board;

    // Temporizadores para animaciones
    float stateTimer = 0.f;
    const float CLEAR_DELAY = 0.3f;
    const float FALL_DELAY = 0.2f;

    // Selección
    int selR = -1, selC = -1;
    int swapR = -1, swapC = -1; // Para revertir si es inválido

    // Helpers
    void updateHUD();
    static void coverSpriteToWindow(sf::Sprite& spr, const sf::Vector2u& win);
};