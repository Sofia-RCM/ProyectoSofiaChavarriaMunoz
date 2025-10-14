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

    srand((unsigned)time(0));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            string tipo;
            bool valido;

            do {
                valido = true;
                tipo = tipos[rand() % 5];


                if (j >= 2 &&
                    matrix[i][j - 1].getTipoGem() == tipo &&
                    matrix[i][j - 2].getTipoGem() == tipo) {
                    valido = false;
                }


                if (i >= 2 &&
                    matrix[i - 1][j].getTipoGem() == tipo &&
                    matrix[i - 2][j].getTipoGem() == tipo) {
                    valido = false;
                }
            } while (!valido);

            Gem g;
            g.setTipoGem(tipo);
            g.setGrid(i, j, CELL);
            setGem(i, j, g);
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

            float cx = j * CELL + offsetX + CELL / 2;
            float cy = i * CELL + offsetY + CELL / 2;
            matrix[i][j].setPosition(cx, cy);
            matrix[i][j].draw(window);
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

    Gem tmp = matrix[r1][c1];
    matrix[r1][c1] = matrix[r2][c2];
    matrix[r2][c2] = tmp;

    matrix[r1][c1].setGrid(r1, c1, CELL);
    matrix[r2][c2].setGrid(r2, c2, CELL);
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

bool Board::isAdjacent(int r1, int c1, int r2, int c2) {
    int dr = r1 - r2; if (dr < 0) dr = -dr;
    int dc = c1 - c2; if (dc < 0) dc = -dc;
    return (dr + dc) == 1;
}

int Board::countRunRow(Gem matrix[8][8], int r, int c) {
    if (matrix[r][c].isEmpty()) return 0;
    string tipo = matrix[r][c].getTipoGem();

    int left = c;
    while (left - 1 >= 0 && !matrix[r][left - 1].isEmpty() &&
        matrix[r][left - 1].getTipoGem() == tipo) {
        --left;
    }
    int right = c;
    while (right + 1 < 8 && !matrix[r][right + 1].isEmpty() &&
        matrix[r][right + 1].getTipoGem() == tipo) {
        ++right;
    }
    return right - left + 1;
}

int Board::countRunCol(Gem matrix[8][8], int r, int c) {
    if (matrix[r][c].isEmpty()) return 0;
    string tipo = matrix[r][c].getTipoGem();

    int up = r;
    while (up - 1 >= 0 && !matrix[up - 1][c].isEmpty() &&
        matrix[up - 1][c].getTipoGem() == tipo) {
        --up;
    }
    int down = r;
    while (down + 1 < 8 && !matrix[down + 1][c].isEmpty() &&
        matrix[down + 1][c].getTipoGem() == tipo) {
        ++down;
    }
    return down - up + 1;
}

bool Board::wouldFormMatch(int r1, int c1, int r2, int c2) {
    if (!isAdjacent(r1, c1, r2, c2)) return false;

    Gem tmp = matrix[r1][c1];
    matrix[r1][c1] = matrix[r2][c2];
    matrix[r2][c2] = tmp;

    bool forms = false;
    int rs[2] = { r1, r2 };
    int cs[2] = { c1, c2 };

    for (int i = 0; i < 2 && !forms; ++i) {
        int r = rs[i], c = cs[i];
        if (r >= 0 && r < 8 && c >= 0 && c < 8) {
            if (countRunRow(matrix, r, c) >= 3) forms = true;
            if (countRunCol(matrix, r, c) >= 3) forms = true;
        }
    }

    tmp = matrix[r1][c1];
    matrix[r1][c1] = matrix[r2][c2];
    matrix[r2][c2] = tmp;

    return forms;
}

int Board::findMatches(bool marks[8][8]) {
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c)
            marks[r][c] = false;

    int totalMarked = 0;

    for (int r = 0; r < N; ++r) {
        int c = 0;
        while (c < N) {
            if (!matrix[r][c].isLoaded()) { c++; continue; }

            string tipo = matrix[r][c].getTipoGem();
            int k = c + 1;
            while (k < N && matrix[r][k].isLoaded() && matrix[r][k].getTipoGem() == tipo) {
                k++;
            }
            int len = k - c;
            if (len >= 3) {
                for (int x = c; x < k; x++) {
                    if (!marks[r][x]) { marks[r][x] = true; totalMarked++; }
                }
            }
            c = (len > 1 ? k : c + 1);
        }
    }

    for (int c = 0; c < N; ++c) {
        int r = 0;
        while (r < N) {
            if (!matrix[r][c].isLoaded()) { r++; continue; }

            string tipo = matrix[r][c].getTipoGem();
            int k = r + 1;
            while (k < N && matrix[k][c].isLoaded() && matrix[k][c].getTipoGem() == tipo) {
                k++;
            }
            int len = k - r;
            if (len >= 3) {
                for (int x = r; x < k; x++) {
                    if (!marks[x][c]) { marks[x][c] = true; totalMarked++; }
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
                cleared++;
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
                write--;
            }
        }
        while (write >= 0) {
            int idx = rand() % 5;
            Gem g;
            string t = tipos[idx];
            g.setTipoGem(t);
            g.setGrid(write, c, CELL);
            matrix[write][c] = g;
            write--;
        }
    }
}
bool Board::hasPossibleMoves() {
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
          
            if (c + 1 < N && wouldFormMatch(r, c, r, c + 1)) return true;
           
            if (r + 1 < N && wouldFormMatch(r, c, r + 1, c)) return true;
        }
    }
    return false;
}

void Board::shuffleBoard() {
    vector<string> tiposTemp;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            tiposTemp.push_back(matrix[i][j].getTipoGem());

    srand((unsigned)time(0));
    for (size_t i = 0; i < tiposTemp.size(); ++i) {
        int randIndex = rand() % tiposTemp.size();
        swap(tiposTemp[i], tiposTemp[randIndex]);
    }
    int index = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            string tipo = tiposTemp[index++];
            Gem g;
            g.setTipoGem(tipo);
            g.setGrid(i, j, CELL);
            setGem(i, j, g);
        }
    }

    while (findAndClearMatches() > 0 || !hasPossibleMoves()) {
        tiposTemp.clear();
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                tiposTemp.push_back(tipos[rand() % 5]);

        for (size_t i = 0; i < tiposTemp.size(); ++i) {
            int randIndex = rand() % tiposTemp.size();
            swap(tiposTemp[i], tiposTemp[randIndex]);
        }

        index = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                string tipo = tiposTemp[index++];
                Gem g;
                g.setTipoGem(tipo);
                g.setGrid(i, j, CELL);
                setGem(i, j, g);
            }
        }
    }
}
