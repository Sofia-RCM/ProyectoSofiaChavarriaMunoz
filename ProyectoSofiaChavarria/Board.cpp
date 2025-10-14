#include "Board.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
using namespace sf;

Board::Board() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            matrix[i][j] = nullptr;
}

Board::~Board() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            delete matrix[i][j];
}

void Board::setGem(int fila, int col, Gem* g) {
    if (fila >= 0 && fila < N && col >= 0 && col < N) {
        delete matrix[fila][col];
        matrix[fila][col] = g;
    }
}
void Board::fillBoard() {
    tipos[0] = "Totoro";
    tipos[1] = "Ponyo";
    tipos[2] = "Parti";
    tipos[3] = "Gato";
    tipos[4] = "Galleta";

    srand((unsigned)time(0));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            string tipo;
            bool valido;

            do {
                valido = true;
                tipo = tipos[rand() % 5];
                if (j >= 2 && matrix[i][j - 1] && matrix[i][j - 2]) {
                    if (matrix[i][j - 1]->getTipoGem() == tipo &&
                        matrix[i][j - 2]->getTipoGem() == tipo)
                        valido = false;
                }
                if (i >= 2 && matrix[i - 1][j] && matrix[i - 2][j]) {
                    if (matrix[i - 1][j]->getTipoGem() == tipo &&
                        matrix[i - 2][j]->getTipoGem() == tipo)
                        valido = false;
                }

            } while (!valido);

            delete matrix[i][j];
            NormalGem* g = new NormalGem();
            g->setTipoGem(tipo);
            g->setGrid(i, j, CELL);
            matrix[i][j] = g;
        }
    }

    while (!hasPossibleMoves()) {
        shuffleBoard();
    }
}

void Board::drawBoard(RenderWindow& window) {
    RectangleShape cell(Vector2f(CELL, CELL));
    cell.setOutlineThickness(1);
    cell.setOutlineColor(Color(0, 80, 0));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cell.setPosition(j * CELL + offsetX, i * CELL + offsetY);
            bool dark = ((i + j) % 2 == 1);
            cell.setFillColor(dark ? Color(100, 200, 100) : Color(170, 255, 170));
            window.draw(cell);

            if (matrix[i][j]) {
                float cx = j * CELL + offsetX + CELL / 2;
                float cy = i * CELL + offsetY + CELL / 2;
                matrix[i][j]->setPosition(cx, cy);
                matrix[i][j]->draw(window);
            }
        }
    }
}

int Board::windowSize() { return N * CELL; }

bool Board::screenToCell(int mouseX, int mouseY, int& fila, int& col) {
    int xx = mouseX - offsetX;
    int yy = mouseY - offsetY;
    if (xx < 0 || yy < 0) return false;
    fila = yy / CELL;
    col = xx / CELL;
    return (fila >= 0 && fila < N && col >= 0 && col < N);
}

void Board::swapCells(int r1, int c1, int r2, int c2) {
    if (r1 < 0 || r1 >= N || c1 < 0 || c1 >= N) return;
    if (r2 < 0 || r2 >= N || c2 < 0 || c2 >= N) return;
    if (r1 == r2 && c1 == c2) return;
    Gem* tmp = matrix[r1][c1];
    matrix[r1][c1] = matrix[r2][c2];
    matrix[r2][c2] = tmp;
    if (matrix[r1][c1]) matrix[r1][c1]->setGrid(r1, c1, CELL);
    if (matrix[r2][c2]) matrix[r2][c2]->setGrid(r2, c2, CELL);
}

void Board::drawSelection(RenderWindow& window, int r, int c) {
    if (r < 0 || r >= N || c < 0 || c >= N) return;
    RectangleShape sel(Vector2f(CELL, CELL));
    sel.setPosition(c * CELL + offsetX, r * CELL + offsetY);
    sel.setFillColor(Color(0, 0, 0, 0));
    sel.setOutlineThickness(3);
    sel.setOutlineColor(Color(255, 215, 0));
    window.draw(sel);
}

int Board::findMatches(bool marks[8][8]) {
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c)
            marks[r][c] = false;

    int total = 0;

    for (int r = 0; r < N; ++r) {
        int c = 0;
        while (c < N) {
            if (!matrix[r][c] || !matrix[r][c]->isLoaded()) { c++; continue; }
            string tipo = matrix[r][c]->getTipoGem();
            int k = c + 1;
            while (k < N && matrix[r][k] && matrix[r][k]->getTipoGem() == tipo)
                k++;
            int len = k - c;
            if (len >= 3) {
                for (int x = c; x < k; x++) marks[r][x] = true;
                total += len;
                if (len >= 4) {
                    int mid = (c + k - 1) / 2;
                    delete matrix[r][mid];
                    matrix[r][mid] = new BombGem();
                    matrix[r][mid]->setTipoGem(tipo);
                    matrix[r][mid]->setGrid(r, mid, CELL);
                }
            }
            c = k;
        }
    }

    for (int c = 0; c < N; ++c) {
        int r = 0;
        while (r < N) {
            if (!matrix[r][c] || !matrix[r][c]->isLoaded()) { r++; continue; }
            string tipo = matrix[r][c]->getTipoGem();
            int k = r + 1;
            while (k < N && matrix[k][c] && matrix[k][c]->getTipoGem() == tipo)
                k++;
            int len = k - r;
            if (len >= 3) {
                for (int x = r; x < k; x++) marks[x][c] = true;
                total += len;
                if (len >= 5) {
                    int mid = (r + k - 1) / 2;
                    delete matrix[mid][c];
                    matrix[mid][c] = new IceGem();
                    matrix[mid][c]->setTipoGem(tipo);
                    matrix[mid][c]->setGrid(mid, c, CELL);
                }
            }
            r = k;
        }
    }

    return total;
}

int Board::clearMarked(const bool marks[8][8]) {
    int cleared = 0;
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (marks[r][c] && matrix[r][c]) {
                if (matrix[r][c]->getType() == "Bomb" || matrix[r][c]->getType() == "Ice")
                    continue;
                matrix[r][c]->onMatch();
                delete matrix[r][c];
                matrix[r][c] = nullptr;
                cleared++;
            }
        }
    }
    return cleared;
}

int Board::findAndClearMatches() {
    bool marks[N][N];
    int m = findMatches(marks);
    if (m == 0) return 0;
    return clearMarked(marks);
}

void Board::applyGravityAndRefill() {
    for (int c = 0; c < N; ++c) {
        int write = N - 1;
        for (int r = N - 1; r >= 0; --r) {
            if (matrix[r][c]) {
                if (r != write) {
                    matrix[write][c] = matrix[r][c];
                    matrix[write][c]->setGrid(write, c, CELL);
                    matrix[r][c] = nullptr;
                }
                write--;
            }
        }
        while (write >= 0) {
            int idx = rand() % 5;
            string tipo = tipos[idx];
            NormalGem* g = new NormalGem();
            g->setTipoGem(tipo);
            g->setGrid(write, c, CELL);
            matrix[write][c] = g;
            write--;
        }
    }
}

bool Board::hasPossibleMoves() {
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (!matrix[r][c]) continue;
            if (r + 1 < N && matrix[r + 1][c] && matrix[r][c]->getTipoGem() == matrix[r + 1][c]->getTipoGem())
                return true;
            if (c + 1 < N && matrix[r][c + 1] && matrix[r][c]->getTipoGem() == matrix[r][c + 1]->getTipoGem())
                return true;
        }
    }
    return false;
}

void Board::shuffleBoard() {
    vector<Gem*> gems;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (matrix[i][j]) gems.push_back(matrix[i][j]);

    random_shuffle(gems.begin(), gems.end());

    int k = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (matrix[i][j]) matrix[i][j] = gems[k++];
            if (matrix[i][j]) matrix[i][j]->setGrid(i, j, CELL);
        }
    }
}
