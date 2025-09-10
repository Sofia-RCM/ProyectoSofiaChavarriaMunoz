#pragma once
#include <string>
#include "Gem.h"
using namespace std;
using namespace sf;

namespace { class RenderWindow; }


class Board {
private:
    static const int N = 8;
    static const int CELL = 110;
    Gem matrix[N][N];
    string tipos[5];
    string imagenes[5];

public:
    Board();

    void setGem(int fila, int col, Gem g);
    void fillBoard();
    void drawBoard(sf::RenderWindow& window);
    int windowSize();
    bool screenToCell(int mouseX, int mouseY, int& fila, int& col);
    void swapCells(int r1, int c1, int r2, int c2);
    void drawSelection(sf::RenderWindow& window, int r, int c);

    int findMatches(bool marks[8][8]);
    int clearMarked(const bool marks[8][8]);
    int findAndClearMatches();
    void applyGravityAndRefill();
};
