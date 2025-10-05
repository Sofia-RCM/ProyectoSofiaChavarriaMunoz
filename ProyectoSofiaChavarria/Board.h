#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Gem.h"
#include "Constants.h"
using namespace std;
using namespace sf;

class Board {
private:
    Gem matrix[BOARD_SIZE][BOARD_SIZE];
    string tipos[5];
    int offsetX = 0;
    int offsetY = 0;

public:
    Board();

    void setGem(int fila, int col, Gem g);
    void fillBoard();
    void drawBoard(RenderWindow& window);
    int windowSize();
    bool screenToCell(int mouseX, int mouseY, int& fila, int& col);
    void swapCells(int r1, int c1, int r2, int c2);
    void drawSelection(RenderWindow& window, int r, int c);

    int findMatches(bool marks[BOARD_SIZE][BOARD_SIZE]);
    int clearMarked(const bool marks[BOARD_SIZE][BOARD_SIZE]);
    int findAndClearMatches();
    void applyGravityAndRefill();

    void setOffset(int x, int y) { offsetX = x; offsetY = y; }
};
