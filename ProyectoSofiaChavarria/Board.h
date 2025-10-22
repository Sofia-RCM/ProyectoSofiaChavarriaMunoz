#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Gem.h"

class Board {
public:
    static const int N = 8;          // tamaño del tablero (8x8)
    static const int CELL = 96;      // tamaño de cada celda

private:
    Gem* matrix[N][N] = { nullptr }; // matriz principal
    sf::Vector2f offset{ 40.f, 80.f };

    // --- Estructura para animación de caída ---
    struct AnimData {
        sf::Vector2f startPos;
        sf::Vector2f endPos;
        float progress = 0.f;
        bool active = false;
    };
    AnimData animMatrix[N][N];
    float fallSpeed = 3.5f; // velocidad de caída animada

    // --- Estructura para explosiones ---
    struct Explosion {
        sf::Vector2f position;
        float radius = 0.f;
        float lifetime = 0.f;
        bool active = false;
    };
    std::vector<Explosion> explosions;

    // --- Estructura para destellos (flashes) ---
    struct Flash {
        sf::Vector2f position;
        float timer = 0.f;
        bool active = false;
    };
    std::vector<Flash> flashes;

    // Contadores enlazados con Game
    int* pTotoroCount = nullptr;
    int* pIceCount = nullptr;
    int* pPonyoCount = nullptr;

void createSpecialAt(int r, int c, const std::string& tipoBase);
public:
    Board();
    ~Board();

    // --- Configuración del tablero ---
    void setOffset(float x, float y);
    void centerInWindow(int winW, int winH, int hud = 50);

    // --- Carga inicial ---
    void fillBoard();

    // --- Coincidencias y física ---
    int findAndClearMatches();
    int applyGravityAndRefill();
    bool isSwapValid(int r1, int c1, int r2, int c2);
    int swapCells(int r1, int c1, int r2, int c2);

    // --- Dibujo ---
    void drawBoard(sf::RenderWindow& window);
    void drawSelection(sf::RenderWindow& window, int fila, int columna);

    // --- Animaciones ---
    bool anyAnimationActive() const;
    void updateAnimations(float dt);

    // Explosiones
    void updateExplosions(float dt);
    void drawExplosions(sf::RenderWindow& window);

    // Flashes
    void updateFlashes(float dt);
    void drawFlashes(sf::RenderWindow& window);

    // --- Gestión de gemas ---
    bool isValid(int r, int c) const;
    void setGem(int r, int c, Gem* g);
    void clearCell(int r, int c);
    Gem* getGem(int r, int c) const;
    bool screenToCell(int x, int y, int& fila, int& col);

    // --- Limpieza y generación de obstáculos ---
    bool markForClear(int r, int c);
    void clearMarked();
    void placeRandomIce();

    // --- Control de objetivos ---
    void setCounters(int* t, int* i, int* p) {
        pTotoroCount = t;
        pIceCount = i;
        pPonyoCount = p;
    }

    // --- Datos de diseño ---
    int windowSize() const { return N * CELL; }
    int cellSize() const { return CELL; }
    sf::Vector2f getOffset() const { return offset; }


    
};
