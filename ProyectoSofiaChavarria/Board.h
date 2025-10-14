#pragma once
#include <string>
#include "Gem.h"
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

class Board {
private:
    static const int N = 8;
    static const int CELL = 110;
    Gem matrix[N][N];
    string tipos[5];
    int offsetX = 0;
    int offsetY = 0;

    static int countRunRow(Gem matrix[8][8], int r, int c);
    static int countRunCol(Gem matrix[8][8], int r, int c);

public:
    Board();

    void setGem(int fila, int col, Gem g);
    void fillBoard();
    void drawBoard(RenderWindow& window);
    int windowSize();
    bool screenToCell(int mouseX, int mouseY, int& fila, int& col);
    void swapCells(int r1, int c1, int r2, int c2);
    void drawSelection(RenderWindow& window, int r, int c);

    int findMatches(bool marks[8][8]);
    int clearMarked(const bool marks[8][8]);
    int findAndClearMatches();
    void applyGravityAndRefill();

    static bool isAdjacent(int r1, int c1, int r2, int c2);
    bool wouldFormMatch(int r1, int c1, int r2, int c2);

    void setOffset(int x, int y) { offsetX = x; offsetY = y; }
    bool hasPossibleMoves();
    void shuffleBoard();

};
