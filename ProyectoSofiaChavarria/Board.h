#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Gem.h"

class Board {
public:
    static const int N = 8;
    static const int CELL = 96;

private:
    Gem* matrix[N][N] = { nullptr };
    sf::Vector2f offset{ 40.f, 80.f };

    struct Explosion {
        sf::Vector2f position;
        float radius = 0.f;
        float lifetime = 0.f;
        bool active = false;
    };
    std::vector<Explosion> explosions;
    bool marked[N][N] = { false };

public:
    Board();
    ~Board();

    void setOffset(float x, float y);
    void centerInWindow(int winW, int winH, int hud = 50);

    void fillBoard();
    int findAndClearMatches();
    int applyGravityAndRefill();
    int swapCells(int r1, int c1, int r2, int c2);
    bool isSwapValid(int r1, int c1, int r2, int c2);

    void drawBoard(sf::RenderWindow& window);
    void drawSelection(sf::RenderWindow& window, int fila, int columna);
    void updateExplosions(float dt);
    void drawExplosions(sf::RenderWindow& window);

    bool isValid(int i, int j) const;
    void setGem(int i, int j, Gem* g);
    void clearCell(int i, int j);
    Gem* getGem(int r, int c) const { return isValid(r, c) ? matrix[r][c] : nullptr; }
    bool screenToCell(int x, int y, int& fila, int& col);

    void markForClear(int r, int c);
    void clearMarked();

    int windowSize() const { return N * CELL; }
    int cellSize() const { return CELL; }
    sf::Vector2f getOffset() const { return offset; }
};