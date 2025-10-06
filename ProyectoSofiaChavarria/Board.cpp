#include "Board.h"
#include <cstdlib>
#include <ctime>
using namespace std;
using namespace sf;

Board::Board() {
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            matrix[i][j] = nullptr;
}

Board::~Board() {
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            delete matrix[i][j];
}

void Board::setGem(int fila, int col, Gem* g) {
    if (fila >= 0 && fila < BOARD_SIZE && col >= 0 && col < BOARD_SIZE)
        matrix[fila][col] = g;
}

void Board::fillBoard() {
    tipos[0] = "Totoro";
    tipos[1] = "Ponyo";
    tipos[2] = "Parti";
    tipos[3] = "Gato";
    tipos[4] = "Galleta";
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            delete matrix[i][j];
            int r = rand() % 5;
            matrix[i][j] = new NormalGem(tipos[r]);
            matrix[i][j]->setGrid(i, j, CELL_SIZE);
        }
    }
}

void Board::drawBoard(RenderWindow& window) {
    RectangleShape cell(Vector2f(static_cast<float>(CELL_SIZE), static_cast<float>(CELL_SIZE)));
    cell.setOutlineThickness(1);
    cell.setOutlineColor(Color(0, 80, 0));
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            cell.setPosition(static_cast<float>(j * CELL_SIZE + offsetX), static_cast<float>(i * CELL_SIZE + offsetY));
            bool dark = ((i + j) % 2 == 1);
            cell.setFillColor(dark ? Color(100, 200, 100) : Color(170, 255, 170));
            window.draw(cell);
            if (matrix[i][j]) {
                float cx = static_cast<float>(j * CELL_SIZE + offsetX + CELL_SIZE / 2);
                float cy = static_cast<float>(i * CELL_SIZE + offsetY + CELL_SIZE / 2);
                matrix[i][j]->setPosition(cx, cy);
                matrix[i][j]->draw(window);
            }
        }
    }
}

int Board::windowSize() { return BOARD_SIZE * CELL_SIZE; }

bool Board::screenToCell(int mouseX, int mouseY, int& fila, int& col) {
    int xx = mouseX - offsetX;
    int yy = mouseY - offsetY;
    if (xx < 0 || yy < 0) return false;
    fila = yy / CELL_SIZE;
    col = xx / CELL_SIZE;
    return (fila >= 0 && fila < BOARD_SIZE && col >= 0 && col < BOARD_SIZE);
}

void Board::swapCells(int r1, int c1, int r2, int c2) {
    if (r1 < 0 || r1 >= BOARD_SIZE || c1 < 0 || c1 >= BOARD_SIZE) return;
    if (r2 < 0 || r2 >= BOARD_SIZE || c2 < 0 || c2 >= BOARD_SIZE) return;
    if (r1 == r2 && c1 == c2) return;
    Gem* tmp = matrix[r1][c1];
    matrix[r1][c1] = matrix[r2][c2];
    matrix[r2][c2] = tmp;
    matrix[r1][c1]->setGrid(r1, c1, CELL_SIZE);
    matrix[r2][c2]->setGrid(r2, c2, CELL_SIZE);
}

void Board::drawSelection(RenderWindow& window, int r, int c) {
    if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE) return;
    RectangleShape sel(Vector2f(static_cast<float>(CELL_SIZE), static_cast<float>(CELL_SIZE)));
    sel.setPosition(static_cast<float>(c * CELL_SIZE + offsetX), static_cast<float>(r * CELL_SIZE + offsetY));
    sel.setFillColor(Color(0, 0, 0, 0));
    sel.setOutlineThickness(3);
    sel.setOutlineColor(Color(255, 215, 0));
    window.draw(sel);
}

int Board::findMatches(bool marks[BOARD_SIZE][BOARD_SIZE], int lens[BOARD_SIZE][BOARD_SIZE]) {
    for (int r = 0; r < BOARD_SIZE; ++r)
        for (int c = 0; c < BOARD_SIZE; ++c)
            marks[r][c] = false, lens[r][c] = 0;
    int totalMarked = 0;
    for (int r = 0; r < BOARD_SIZE; ++r) {
        int c = 0;
        while (c < BOARD_SIZE) {
            if (!matrix[r][c] || !matrix[r][c]->isLoaded()) { c++; continue; }
            string tipo = matrix[r][c]->getType();
            int k = c + 1;
            while (k < BOARD_SIZE && matrix[r][k] && matrix[r][k]->isLoaded() && matrix[r][k]->getType() == tipo) k++;
            int len = k - c;
            if (len >= 3) {
                for (int x = c; x < k; x++) marks[r][x] = true, lens[r][x] = len;
                totalMarked += len;
            }
            c = (len > 1 ? k : c + 1);
        }
    }
    for (int c = 0; c < BOARD_SIZE; ++c) {
        int r = 0;
        while (r < BOARD_SIZE) {
            if (!matrix[r][c] || !matrix[r][c]->isLoaded()) { r++; continue; }
            string tipo = matrix[r][c]->getType();
            int k = r + 1;
            while (k < BOARD_SIZE && matrix[k][c] && matrix[k][c]->isLoaded() && matrix[k][c]->getType() == tipo) k++;
            int len = k - r;
            if (len >= 3) {
                for (int x = r; x < k; x++) marks[x][c] = true, lens[x][c] = len;
                totalMarked += len;
            }
            r = (len > 1 ? k : r + 1);
        }
    }
    return totalMarked;
}

void Board::createSpecialGem(int r, int c, const string& tipo) {
    delete matrix[r][c];
    if (rand() % 2 == 0)
        matrix[r][c] = new BombGem("Bomb");
    else
        matrix[r][c] = new IceGem();
    matrix[r][c]->setGrid(r, c, CELL_SIZE);
}

int Board::clearMarked(const bool marks[BOARD_SIZE][BOARD_SIZE]) {
    int cleared = 0;
    for (int r = 0; r < BOARD_SIZE; ++r)
        for (int c = 0; c < BOARD_SIZE; ++c)
            if (marks[r][c]) {
                delete matrix[r][c];
                matrix[r][c] = nullptr;
                cleared++;
            }
    return cleared;
}

int Board::findAndClearMatches() {
    bool marks[BOARD_SIZE][BOARD_SIZE];
    int lens[BOARD_SIZE][BOARD_SIZE];
    int marked = findMatches(marks, lens);
    if (marked == 0) return 0;
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (marks[r][c] && lens[r][c] >= 4) {
                createSpecialGem(r, c, matrix[r][c] ? matrix[r][c]->getType() : "");
            }
        }
    }
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (marks[r][c] && matrix[r][c] && matrix[r][c]->isSpecial()) {
                matrix[r][c]->onMatch(*this);
            }
        }
    }
    return clearMarked(marks);
}


void Board::applyGravityAndRefill() {
    for (int c = 0; c < BOARD_SIZE; ++c) {
        int write = BOARD_SIZE - 1;
        for (int r = BOARD_SIZE - 1; r >= 0; --r) {
            if (matrix[r][c] && !matrix[r][c]->isEmpty()) {
                if (r != write) {
                    matrix[write][c] = matrix[r][c];
                    matrix[write][c]->setGrid(write, c, CELL_SIZE);
                    matrix[r][c] = nullptr;
                }
                write--;
            }
        }
        while (write >= 0) {
            int idx = rand() % 5;
            matrix[write][c] = new NormalGem(tipos[idx]);
            matrix[write][c]->setGrid(write, c, CELL_SIZE);
            write--;
        }
    }
}
void Board::removeGem(int r, int c) {
    if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE) return;
    delete matrix[r][c];
    matrix[r][c] = nullptr;
}
