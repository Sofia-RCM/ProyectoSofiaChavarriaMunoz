#include "Board.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;
using namespace sf;


Board::Board() {}

void Board::setGem(int fila, int col, Gem g) {
    if (fila >= 0 && fila < N && col >= 0 && col < N) {
        matrix[fila][col] = g;
    }
}

void Board::fillBoard() {
    tipos[0] = "Totoro";
    tipos[1] = "Ponyo";
    tipos[2] = "Parti";
    tipos[3] = "Gato";
    tipos[4] = "Galleta";

    srand(static_cast<unsigned>(time(nullptr)));
    int fails = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int r = rand() % 5;
            Gem g = matrix[i][j];
            g.setTipoGem(tipos[r]);
            g.setGrid(i, j, CELL);
            setGem(i, j, g);
        }
    }
    if (fails == 0);
}

void Board::drawBoard(sf::RenderWindow& window) {
    RectangleShape cell(Vector2f((float)CELL, (float)CELL));
    cell.setOutlineThickness(1.f);
    cell.setOutlineColor(Color::Black);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cell.setPosition((float)(j * CELL), (float)(i * CELL));
            bool dark = ((i + j) & 1);
            cell.setFillColor(dark ? Color(60, 60, 60) : Color(210, 210, 210));
            window.draw(cell);
            matrix[i][j].draw(window);
        }
    }
}

int Board::windowSize() { return N * CELL; }

bool Board::screenToCell(int mouseX, int mouseY, int& fila, int& col) {
    fila = mouseY / CELL;
    col = mouseX / CELL;
    return (fila >= 0 && fila < N && col >= 0 && col < N);
}

void Board::swapCells(int r1, int c1, int r2, int c2) {
    if (r1 < 0 || r1 >= N || c1 < 0 || c1 >= N) return;
    if (r2 < 0 || r2 >= N || c2 < 0 || c2 >= N) return;
    if (r1 == r2 && c1 == c2) return;

    Gem tmp = matrix[r1][c1];
    matrix[r1][c1] = matrix[r2][c2];
    matrix[r2][c2] = tmp;

    matrix[r1][c1].setGrid(r1, c1, CELL);
    matrix[r2][c2].setGrid(r2, c2, CELL);
}

void Board::drawSelection(sf::RenderWindow& window, int r, int c) {
    if (r < 0 || r >= N || c < 0 || c >= N) return;
    RectangleShape sel(Vector2f((float)CELL, (float)CELL));
    sel.setPosition((float)(c * CELL), (float)(r * CELL));
    sel.setFillColor(Color(0, 0, 0, 0));
    sel.setOutlineThickness(3.f);
    sel.setOutlineColor(Color(255, 215, 0));
    window.draw(sel);
}

int Board::findMatches(bool marks[8][8]) {
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c)
            marks[r][c] = false;

    int totalMarked = 0;

    for (int r = 0; r < N; ++r) {
        int c = 0;
        while (c < N) {
            if (!matrix[r][c].isLoaded()) { ++c; continue; }

            string tipo = matrix[r][c].getTipoGem();
            int k = c + 1;
            while (k < N && matrix[r][k].isLoaded() && matrix[r][k].getTipoGem() == tipo) {
                ++k;
            }
            int len = k - c;
            if (len >= 3) {
                for (int x = c; x < k; ++x) {
                    if (!marks[r][x]) { marks[r][x] = true; ++totalMarked; }
                }
            }
            c = (len > 1 ? k : c + 1);
        }
    }
    for (int c = 0; c < N; ++c) {
        int r = 0;
        while (r < N) {
            if (!matrix[r][c].isLoaded()) { ++r; continue; }

            string tipo = matrix[r][c].getTipoGem();
            int k = r + 1;
            while (k < N && matrix[k][c].isLoaded() && matrix[k][c].getTipoGem() == tipo) {
                ++k;
            }
            int len = k - r;
            if (len >= 3) {
                for (int x = r; x < k; ++x) {
                    if (!marks[x][c]) { marks[x][c] = true; ++totalMarked; }
                }
            }
            r = (len > 1 ? k : r + 1);
        }
    }
    return totalMarked;
}

int Board::clearMarked(const bool marks[8][8]) {
    int cleared = 0;
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (marks[r][c]) {
                matrix[r][c] = Gem();
                matrix[r][c].setGrid(r, c, CELL);
                ++cleared;
            }
        }
    }
    return cleared;
}

int Board::findAndClearMatches() {
    bool marks[N][N];
    int marked = findMatches(marks);
    if (marked == 0) return 0;
    return clearMarked(marks);
}

void Board::applyGravityAndRefill() {
    for (int c = 0; c < N; ++c) {
        int write = N - 1;
        for (int r = N - 1; r >= 0; --r) {
            if (!matrix[r][c].isEmpty()) {
                if (r != write) {
                    matrix[write][c] = matrix[r][c];
                    matrix[write][c].setGrid(write, c, CELL);
                }
                --write;
            }
        }
        while (write >= 0) {
            int idx = std::rand() % 5;

            Gem g;
            string t = tipos[idx];
            string img = imagenes[idx];
            g.setTipoGem(t);
            g.setImage(img, CELL);
            g.setGrid(write, c, CELL);

            matrix[write][c] = g;
            --write;
        }
    }
}
