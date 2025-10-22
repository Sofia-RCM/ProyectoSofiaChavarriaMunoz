#include "Board.h"
#include "TotoroGem.h"
#include "PonyoGem.h"
#include "PartiGem.h"
#include "GatoGem.h"
#include "GalletaGem.h"
#include "IceGem.h"
#include "NormalGem.h"
#include <cstdlib>
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

bool Board::isValid(int r, int c) const {
    return (r >= 0 && r < N && c >= 0 && c < N);
}

void Board::setGem(int r, int c, Gem* g) {
    if (!isValid(r, c)) return;
    matrix[r][c] = g;
}

void Board::clearCell(int r, int c) {
    if (!isValid(r, c)) return;
    delete matrix[r][c];
    matrix[r][c] = nullptr;
}

void Board::fillBoard() {
    const std::string tipos[6] = { "Totoro","Ponyo","Parti","Gato","Galleta","Ice" };

    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {

            std::string tipo;
            bool ok;
            do {
                ok = true;
                tipo = tipos[std::rand() % 6];

                if (tipo != "Ice") {
                    if (c >= 2 && matrix[r][c - 1] && matrix[r][c - 2] &&
                        matrix[r][c - 1]->getTipoGem() == tipo &&
                        matrix[r][c - 2]->getTipoGem() == tipo) ok = false;
                    if (r >= 2 && matrix[r - 1][c] && matrix[r - 2][c] &&
                        matrix[r - 1][c]->getTipoGem() == tipo &&
                        matrix[r - 2][c]->getTipoGem() == tipo) ok = false;
                }
            } while (!ok);

            Gem* g = nullptr;
            if (tipo == "Totoro")        g = new TotoroGem();
            else if (tipo == "Ponyo")    g = new PonyoGem();
            else if (tipo == "Parti")    g = new PartiGem();
            else if (tipo == "Gato")     g = new GatoGem();
            else if (tipo == "Galleta")  g = new GalletaGem();
            else if (tipo == "Ice")      g = new IceGem();
            else                         g = new NormalGem();

            g->setTipoGem(tipo);
            g->setGrid(r, c, CELL, offset.x, offset.y);
            setGem(r, c, g);
        }
    }
}

void Board::createSpecialAt(int r, int c, const std::string& tipoBase) {
    delete matrix[r][c];
    Gem* s = nullptr;
    if (tipoBase == "Totoro")       s = new TotoroGem();
    else if (tipoBase == "Ponyo")   s = new PonyoGem();
    else if (tipoBase == "Parti")   s = new PartiGem();
    else if (tipoBase == "Gato")    s = new GatoGem();
    else if (tipoBase == "Galleta") s = new GalletaGem();
    else                            s = new NormalGem();

    s->setTipoGem(tipoBase + "Especial");
    s->setGrid(r, c, CELL, offset.x, offset.y);
    matrix[r][c] = s;
}

int Board::findAndClearMatches() {
    bool mark[N][N] = { false };
    int cleared = 0;

    // Horizontales
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
                        if (matrix[r][center]) {
                            std::string base = matrix[r][center]->getTipoGem();
                            auto pos = base.find("Especial");
                            if (pos != std::string::npos) base = base.substr(0, pos);
                            if (base != "Ice") {
                                mark[r][center] = false;
                                createSpecialAt(r, center, base);
                            }
                        }
                    }
                }
                count = 1;
            }
        }
    }

    // Verticales
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
                        if (matrix[center][c]) {
                            std::string base = matrix[center][c]->getTipoGem();
                            auto pos = base.find("Especial");
                            if (pos != std::string::npos) base = base.substr(0, pos);
                            if (base != "Ice") {
                                mark[center][c] = false;
                                createSpecialAt(center, c, base);
                            }
                        }
                    }
                }
                count = 1;
            }
        }
    }

    // Eliminar marcados
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (mark[r][c] && matrix[r][c]) {
                matrix[r][c]->onMatch(*this, r, c);
                markForClear(r, c);
                ++cleared;
            }
        }
    }
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
                    if (tipo == "Totoro")       g = new TotoroGem();
                    else if (tipo == "Ponyo")   g = new PonyoGem();
                    else if (tipo == "Parti")   g = new PartiGem();
                    else if (tipo == "Gato")    g = new GatoGem();
                    else if (tipo == "Galleta") g = new GalletaGem();
                    else                        g = new NormalGem();

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

bool Board::isSwapValid(int r1, int c1, int r2, int c2) {
    if (!isValid(r1, c1) || !isValid(r2, c2)) return false;
    Gem* g1 = matrix[r1][c1];
    Gem* g2 = matrix[r2][c2];
    if (!g1 || !g2) return false;

    if (dynamic_cast<IceGem*>(g1) || dynamic_cast<IceGem*>(g2))
        return false;

    std::swap(matrix[r1][c1], matrix[r2][c2]);

    bool found = false;

    for (int r : {r1, r2}) {
        int count = 1;
        for (int c = 1; c < N; ++c) {
            if (matrix[r][c] && matrix[r][c - 1] &&
                matrix[r][c]->getTipoGem() == matrix[r][c - 1]->getTipoGem()) {
                if (++count >= 3) { found = true; break; }
            }
            else count = 1;
        }
        if (found) break;
    }

    if (!found) {
        for (int c : {c1, c2}) {
            int count = 1;
            for (int r = 1; r < N; ++r) {
                if (matrix[r][c] && matrix[r - 1][c] &&
                    matrix[r][c]->getTipoGem() == matrix[r - 1][c]->getTipoGem()) {
                    if (++count >= 3) { found = true; break; }
                }
                else count = 1;
            }
            if (found) break;
        }
    }

    std::swap(matrix[r1][c1], matrix[r2][c2]);
    return found;
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
    sf::RectangleShape cell({ (float)CELL - 6, (float)CELL - 6 });
    cell.setFillColor(sf::Color(255, 255, 255, 215));
    cell.setOutlineColor(sf::Color(220, 220, 220));
    cell.setOutlineThickness(3);

    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            cell.setPosition(offset.x + c * CELL + 3, offset.y + r * CELL + 3);
            window.draw(cell);
        }
    }

    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c)
            if (matrix[r][c] && !matrix[r][c]->isEmpty())
                matrix[r][c]->draw(window);

    drawExplosions(window);
}

void Board::drawSelection(sf::RenderWindow& window, int fila, int columna) {
    if (!isValid(fila, columna)) return;
    sf::RectangleShape rect({ (float)CELL,(float)CELL });
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

bool Board::markForClear(int r, int c) {
    if (isValid(r, c) && matrix[r][c]) {
        std::string tipo = matrix[r][c]->getTipoGem();

        if (pTotoroCount && tipo == "Totoro") {
            (*pTotoroCount)++;
        }
        if (pPonyoCount && tipo == "Ponyo") {
            (*pPonyoCount)++;
        }
        if (pIceCount && tipo == "Ice") {
            (*pIceCount)++;
        }

        IceGem* ice = dynamic_cast<IceGem*>(matrix[r][c]);
        if (ice && !ice->isBroken()) {
            ice->receiveHit();
            if (ice->isBroken()) {
                delete matrix[r][c];
                matrix[r][c] = nullptr;
                return true;
            }
            return false;
        }
        else {
            delete matrix[r][c];
            matrix[r][c] = nullptr;
            return false;
        }
    }
    return false;
}

void Board::clearMarked() {
    // ya no usado
}

void Board::placeRandomIce() {
    for (int tries = 0; tries < 50; ++tries) {
        int r = std::rand() % N;
        int c = std::rand() % N;
        if (!matrix[r][c]) continue;
        if (matrix[r][c]->getTipoGem().find("Ice") != std::string::npos) continue;

        delete matrix[r][c];
        Gem* ice = new IceGem();
        ice->setTipoGem("Ice");
        ice->setGrid(r, c, CELL, offset.x, offset.y);
        matrix[r][c] = ice;
        return;
    }
}