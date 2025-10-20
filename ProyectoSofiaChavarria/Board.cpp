#include "Board.h"
#include "TotoroGem.h"
#include "PonyoGem.h"
#include "PartiGem.h"
#include "GatoGem.h"
#include "GalletaGem.h"
#include <ctime>
#include <iostream>

Board::Board() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    fillBoard();
}

Board::~Board() {
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c)
            delete matrix[r][c];
}

void Board::setOffset(float x, float y) {
    offset = { x, y };
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c)
            if (matrix[r][c])
                matrix[r][c]->setGrid(r, c, CELL, offset.x, offset.y);
}

void Board::centerInWindow(int winW, int winH, int hud) {
    const int boardW = N * CELL;
    const int boardH = N * CELL;
    float offX = (winW - boardW) * 0.5f;
    float offY = hud + (winH - hud - boardH) * 0.5f;
    setOffset(offX, offY);
}

bool Board::isValid(int i, int j) const {
    return (i >= 0 && i < N && j >= 0 && j < N);
}

void Board::setGem(int i, int j, Gem* g) {
    if (!isValid(i, j)) return;
    matrix[i][j] = g;
}

void Board::clearCell(int i, int j) {
    if (!isValid(i, j)) return;
    delete matrix[i][j];
    matrix[i][j] = nullptr;
}

void Board::fillBoard() {
    const std::string tipos[5] = { "Totoro","Ponyo","Parti","Gato","Galleta" };
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            std::string tipo;
            bool ok;
            do {
                ok = true;
                tipo = tipos[std::rand() % 5];
                if (c >= 2 && matrix[r][c - 1] && matrix[r][c - 2] &&
                    matrix[r][c - 1]->getTipoGem() == tipo &&
                    matrix[r][c - 2]->getTipoGem() == tipo) ok = false;
                if (r >= 2 && matrix[r - 1][c] && matrix[r - 2][c] &&
                    matrix[r - 1][c]->getTipoGem() == tipo &&
                    matrix[r - 2][c]->getTipoGem() == tipo) ok = false;
            } while (!ok);

            Gem* g = nullptr;
            if (tipo == "Totoro")   g = new TotoroGem();
            else if (tipo == "Ponyo")   g = new PonyoGem();
            else if (tipo == "Parti")   g = new PartiGem();
            else if (tipo == "Gato")    g = new GatoGem();
            else if (tipo == "Galleta") g = new GalletaGem();

            g->setTipoGem(tipo);
            g->setGrid(r, c, CELL, offset.x, offset.y);
            setGem(r, c, g);
        }
    }
}

void Board::markForClear(int r, int c) {
    if (isValid(r, c)) marked[r][c] = true;
}

void Board::clearMarked() {
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c)
            if (marked[r][c]) {
                delete matrix[r][c];
                matrix[r][c] = nullptr;
                marked[r][c] = false;
            }
}

int Board::findAndClearMatches() {
    bool mark[N][N] = { false };
    int cleared = 0;

    // ---- Barrido horizontal ----
    for (int r = 0; r < N; ++r) {
        int count = 1;
        for (int c = 1; c <= N; ++c) {
            bool same = (c < N && matrix[r][c] && matrix[r][c - 1] &&
                matrix[r][c]->getTipoGem() == matrix[r][c - 1]->getTipoGem());
            if (same) {
                ++count;
            }
            else {
                if (count >= 3 && matrix[r][c - 1]) {
                    for (int k = c - count; k < c; ++k) mark[r][k] = true;
                    if (count >= 4) {
                        int center = c - count + count / 2;
                        mark[r][center] = false;
                        std::string tipo = matrix[r][center]->getTipoGem();
                        delete matrix[r][center];
                        Gem* special = nullptr;
                        if (tipo == "Totoro")   special = new TotoroGem();
                        else if (tipo == "Ponyo")   special = new PonyoGem();
                        else if (tipo == "Parti")   special = new PartiGem();
                        else if (tipo == "Gato")    special = new GatoGem();
                        else if (tipo == "Galleta") special = new GalletaGem();
                        if (special) {
                            special->setTipoGem(tipo + "Especial");
                            special->setGrid(r, center, CELL, offset.x, offset.y);
                            matrix[r][center] = special;
                        }
                    }
                }
                count = 1;
            }
        }
    }

    // ---- Barrido vertical ----
    for (int c = 0; c < N; ++c) {
        int count = 1;
        for (int r = 1; r <= N; ++r) {
            bool same = (r < N && matrix[r][c] && matrix[r - 1][c] &&
                matrix[r][c]->getTipoGem() == matrix[r - 1][c]->getTipoGem());
            if (same) {
                ++count;
            }
            else {
                if (count >= 3 && matrix[r - 1][c]) {
                    for (int k = r - count; k < r; ++k) mark[k][c] = true;
                    if (count >= 4) {
                        int center = r - count + count / 2;
                        mark[center][c] = false;
                        std::string tipo = matrix[center][c]->getTipoGem();
                        delete matrix[center][c];
                        Gem* special = nullptr;
                        if (tipo == "Totoro")   special = new TotoroGem();
                        else if (tipo == "Ponyo")   special = new PonyoGem();
                        else if (tipo == "Parti")   special = new PartiGem();
                        else if (tipo == "Gato")    special = new GatoGem();
                        else if (tipo == "Galleta") special = new GalletaGem();
                        if (special) {
                            special->setTipoGem(tipo + "Especial");
                            special->setGrid(center, c, CELL, offset.x, offset.y);
                            matrix[center][c] = special;
                        }
                    }
                }
                count = 1;
            }
        }
    }

    // ---- Ejecutar onMatch en gemas normales ----
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (mark[r][c] && matrix[r][c]) {
                matrix[r][c]->onMatch(*this, r, c);
                delete matrix[r][c];
                matrix[r][c] = nullptr;
                ++cleared;
            }
        }
    }

    // ---- Limpiar marcadas por gemas especiales ----
    clearMarked();

    return cleared;
}

int Board::applyGravityAndRefill() {
    int moved = 0;
    const std::string tipos[5] = { "Totoro","Ponyo","Parti","Gato","Galleta" };
    for (int c = 0; c < N; ++c) {
        for (int r = N - 1; r >= 0; --r) {
            if (matrix[r][c] == nullptr) {
                int k = r - 1;
                while (k >= 0 && matrix[k][c] == nullptr) --k;
                if (k >= 0) {
                    matrix[r][c] = matrix[k][c];
                    matrix[k][c] = nullptr;
                    matrix[r][c]->setGrid(r, c, CELL, offset.x, offset.y);
                    ++moved;
                }
                else {
                    std::string tipo = tipos[std::rand() % 5];
                    Gem* g = nullptr;
                    if (tipo == "Totoro")   g = new TotoroGem();
                    else if (tipo == "Ponyo")   g = new PonyoGem();
                    else if (tipo == "Parti")   g = new PartiGem();
                    else if (tipo == "Gato")    g = new GatoGem();
                    else if (tipo == "Galleta") g = new GalletaGem();
                    g->setTipoGem(tipo);
                    g->setGrid(r, c, CELL, offset.x, offset.y);
                    matrix[r][c] = g;
                    ++moved;
                }
            }
        }
    }
    return moved;
}

int Board::swapCells(int r1, int c1, int r2, int c2) {
    if (!isValid(r1, c1) || !isValid(r2, c2)) return 0;
    std::swap(matrix[r1][c1], matrix[r2][c2]);
    if (matrix[r1][c1]) matrix[r1][c1]->setGrid(r1, c1, CELL, offset.x, offset.y);
    if (matrix[r2][c2]) matrix[r2][c2]->setGrid(r2, c2, CELL, offset.x, offset.y);
    return 1;
}

bool Board::screenToCell(int x, int y, int& fila, int& col) {
    x -= static_cast<int>(offset.x);
    y -= static_cast<int>(offset.y);
    if (x < 0 || y < 0) return false;
    col = x / CELL;
    fila = y / CELL;
    return isValid(fila, col);
}

void Board::drawBoard(sf::RenderWindow& window) {
    sf::RectangleShape cellShape({ (float)CELL - 6, (float)CELL - 6 });
    cellShape.setFillColor(sf::Color(255, 255, 255, 215));
    cellShape.setOutlineColor(sf::Color(220, 220, 220));
    cellShape.setOutlineThickness(3);

    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            cellShape.setPosition(offset.x + c * CELL + 3, offset.y + r * CELL + 3);
            window.draw(cellShape);
        }
    }

    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (matrix[r][c] && !matrix[r][c]->isEmpty())
                matrix[r][c]->draw(window);
        }
    }
    drawExplosions(window);
}

void Board::drawSelection(sf::RenderWindow& window, int fila, int columna) {
    if (!isValid(fila, columna)) return;
    sf::RectangleShape rect({ (float)CELL, (float)CELL });
    rect.setPosition(offset.x + columna * CELL, offset.y + fila * CELL);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Yellow);
    rect.setOutlineThickness(5);
    window.draw(rect);
}

void Board::updateExplosions(float dt) {
    for (auto& e : explosions) {
        if (!e.active) continue;
        e.lifetime -= dt;
        e.radius += 260.f * dt;
        if (e.lifetime <= 0.f) e.active = false;
    }
}

void Board::drawExplosions(sf::RenderWindow& window) {
    for (auto& e : explosions) {
        if (!e.active) continue;
        sf::CircleShape circle(e.radius);
        circle.setOrigin(e.radius, e.radius);
        circle.setPosition(e.position);
        circle.setFillColor(sf::Color(255, 200, 0, 60));
        circle.setOutlineColor(sf::Color(255, 140, 0, 180));
        circle.setOutlineThickness(4);
        window.draw(circle);
    }
}