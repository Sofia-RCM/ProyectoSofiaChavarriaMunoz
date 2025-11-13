#include "Board.h"
#include "TotoroGem.h"
#include "PonyoGem.h"
#include "PartiGem.h"
#include "GatoGem.h"
#include "GalletaGem.h"
#include "IceGem.h"
// ¡OJO!: sin #include "NormalGem.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;
using namespace sf;

// Constructor del tablero
Board::Board() {
    srand(static_cast<unsigned>(time(nullptr)));
    fillBoard();
}

// Destructor
Board::~Board() {
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c)
            delete matrix[r][c];
}

// CONFIGURACIÓN DE TABLERO
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

Gem* Board::getGem(int r, int c) const {
    return isValid(r, c) ? matrix[r][c] : nullptr;
}

// LLENAR TABLERO
void Board::fillBoard() {
    const string tipos[6] = { "Totoro","Ponyo","Parti","Gato","Galleta","Ice" };
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            string tipo;
            bool ok;
            do {
                ok = true;
                tipo = tipos[rand() % 6];
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
            if (tipo == "Totoro") g = new TotoroGem();
            else if (tipo == "Ponyo") g = new PonyoGem();
            else if (tipo == "Parti") g = new PartiGem();
            else if (tipo == "Gato") g = new GatoGem();
            else if (tipo == "Galleta") g = new GalletaGem();
            else if (tipo == "Ice") g = new IceGem();

            g->setTipoGem(tipo);
            g->setGrid(r, c, CELL, offset.x, offset.y);
            setGem(r, c, g);
        }
    }
}

// ANIMACIONES
bool Board::anyAnimationActive() const {
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c)
            if (animMatrix[r][c].active)
                return true;
    return false;
}

void Board::updateAnimations(float dt) {
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c) {
            if (!animMatrix[r][c].active) continue;
            auto& anim = animMatrix[r][c];
            anim.progress += dt * fallSpeed;
            if (anim.progress >= 1.f) {
                anim.progress = 1.f;
                anim.active = false;
            }
            if (matrix[r][c]) {
                Vector2f pos = anim.startPos + (anim.endPos - anim.startPos) * anim.progress;
                matrix[r][c]->getSprite().setPosition(pos);
            }
        }
}

void Board::updateExplosions(float dt) {
    for (auto& e : explosions) {
        if (!e.active) continue;
        e.lifetime -= dt;
        e.radius += 260.f * dt;
        if (e.lifetime <= 0.f) e.active = false;
    }
}

void Board::drawExplosions(RenderWindow& window) {
    for (auto& e : explosions) {
        if (!e.active) continue;
        CircleShape circle(e.radius);
        circle.setOrigin(e.radius, e.radius);
        circle.setPosition(e.position);
        circle.setFillColor(Color(255, 200, 0, 60));
        circle.setOutlineColor(Color(255, 140, 0, 180));
        circle.setOutlineThickness(4);
        window.draw(circle);
    }
}

void Board::updateFlashes(float dt) {
    for (auto& f : flashes) {
        if (!f.active) continue;
        f.timer += dt;
        if (f.timer >= 0.25f) f.active = false;
    }
}

void Board::drawFlashes(RenderWindow& window) {
    for (auto& f : flashes) {
        if (!f.active) continue;
        float progress = f.timer / 0.25f;
        float alpha = 255 * (1.f - progress);
        float radius = 20.f + 40.f * progress;
        CircleShape flash(radius);
        flash.setOrigin(radius, radius);
        flash.setPosition(f.position);
        flash.setFillColor(Color(255, 255, 180, (int)alpha));
        window.draw(flash);
    }
}

// ? NUEVO: EFECTO DE RESALTADO DE CELDAS (no bloqueante)
void Board::startHighlight(const std::vector<std::pair<int, int>>& cells,
    float durationSec, sf::Color color)
{
    highlightCells = cells;
    highlightDuration = (durationSec <= 0.f ? 0.45f : durationSec);
    highlightTimer = 0.f;
    highlightActive = !highlightCells.empty();
    highlightColor = color;
}

void Board::updateHighlight(float dt)
{
    if (!highlightActive) return;
    highlightTimer += dt;
    if (highlightTimer >= highlightDuration) {
        highlightActive = false; // El Game decidirá cuándo borrar
    }
}

void Board::drawHighlight(RenderWindow& window)
{
    if (highlightCells.empty()) return;

    float t = std::min(highlightTimer / std::max(0.0001f, highlightDuration), 1.f);
    int alpha = (int)(highlightColor.a * (1.f - 0.25f * t));
    sf::Color fill = highlightColor; fill.a = alpha;

    sf::RectangleShape rect({ (float)CELL - 6, (float)CELL - 6 });
    rect.setFillColor(fill);
    rect.setOutlineColor(sf::Color(60, 180, 90, alpha));
    rect.setOutlineThickness(3.f);

    for (auto& rc : highlightCells) {
        int r = rc.first, c = rc.second;
        if (!isValid(r, c)) continue;
        rect.setPosition(offset.x + c * CELL + 3, offset.y + r * CELL + 3);
        window.draw(rect);
    }
}

int Board::clearHighlightedCells()
{
    int cleared = 0;
    for (auto& rc : highlightCells) {
        int r = rc.first, c = rc.second;
        if (isValid(r, c)) {
            if (markForClear(r, c)) ++cleared;
        }
    }
    highlightCells.clear();
    highlightTimer = 0.f;
    highlightDuration = 0.f;
    highlightActive = false;
    return cleared;
}

// Crea gema especial
void Board::createSpecialAt(int r, int c, const std::string& tipoBase) {
    delete matrix[r][c];
    Gem* s = nullptr;
    if (tipoBase == "Totoro") s = new TotoroGem();
    else if (tipoBase == "Ponyo") s = new PonyoGem();
    else if (tipoBase == "Parti") s = new PartiGem();
    else if (tipoBase == "Gato") s = new GatoGem();
    else if (tipoBase == "Galleta") s = new GalletaGem();
    else s = new Gem(); // fallback (no debería ocurrir)
    s->setTipoGem(tipoBase + "Especial");
    s->setGrid(r, c, CELL, offset.x, offset.y);
    matrix[r][c] = s;
}

// MATCH Y LIMPIEZA
int Board::findAndClearMatches() {
    bool mark[N][N] = { false };
    int cleared = 0;

    // Horizontales
    for (int r = 0; r < N; ++r) {
        int count = 1;
        for (int c = 1; c <= N; ++c) {
            bool same = (c < N && matrix[r][c] && matrix[r][c - 1] &&
                matrix[r][c]->getTipoGem() == matrix[r][c - 1]->getTipoGem());
            if (same) ++count;
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
            if (same) ++count;
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
    // Eliminar marcadas
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c)
            if (mark[r][c] && matrix[r][c]) {
                matrix[r][c]->onMatch(*this, r, c);

                // 💥 Daño a bloques de hielo adyacentes
                const int dirs[4][2] = { {1,0}, {-1,0}, {0,1}, {0,-1} };
                for (auto& d : dirs) {
                    int nr = r + d[0];
                    int nc = c + d[1];
                    if (nr >= 0 && nr < N && nc >= 0 && nc < N && matrix[nr][nc]) {
                        // Si hay una IceGem adyacente, recibe un golpe
                        if (matrix[nr][nc]->getTipoGem() == "Ice") {
                            IceGem* ice = dynamic_cast<IceGem*>(matrix[nr][nc]);
                            if (ice) {
                                ice->receiveHit();

                                // Si el hielo se rompió, marcarlo para limpiar y regenerar
                                if (ice->isBroken()) {
                                    markForClear(nr, nc);
                                    ++cleared;
                                }
                            }
                        }
                    }
                }


                //  Efecto visual (explosión)
                Explosion e;
                e.position = sf::Vector2f(offset.x + c * CELL + CELL * 0.5f,
                    offset.y + r * CELL + CELL * 0.5f);
                e.radius = 0.f;
                e.lifetime = 0.5f;
                e.active = true;
                explosions.push_back(e);

                markForClear(r, c);
                ++cleared;
            }

    return cleared;

}

// GRAVEDAD + RELLENO
int Board::applyGravityAndRefill() {
    int moved = 0;
    const string tipos[5] = { "Totoro","Ponyo","Parti","Gato","Galleta" };
    for (int c = 0; c < N; ++c) {
        for (int r = N - 1; r >= 0; --r) {
            if (matrix[r][c] == nullptr) {
                int k = r - 1;
                while (k >= 0 && matrix[k][c] == nullptr) --k;
                if (k >= 0) {
                    matrix[r][c] = matrix[k][c];
                    matrix[k][c] = nullptr;
                    Vector2f start = { offset.x + c * CELL, offset.y + k * CELL };
                    Vector2f end = { offset.x + c * CELL, offset.y + r * CELL };
                    matrix[r][c]->setGrid(r, c, CELL, offset.x, offset.y);
                    matrix[r][c]->getSprite().setPosition(start);
                    animMatrix[r][c] = { start, end, 0.f, true };
                    ++moved;
                }
                else {
                    string tipo = tipos[rand() % 5];
                    Gem* g = nullptr;
                    if (tipo == "Totoro") g = new TotoroGem();
                    else if (tipo == "Ponyo") g = new PonyoGem();
                    else if (tipo == "Parti") g = new PartiGem();
                    else if (tipo == "Gato") g = new GatoGem();
                    else if (tipo == "Galleta") g = new GalletaGem();
                    g->setTipoGem(tipo);
                    g->setGrid(r, c, CELL, offset.x, offset.y);
                    Vector2f start = { offset.x + c * CELL, offset.y - CELL };
                    g->getSprite().setPosition(start);
                    animMatrix[r][c] = { start, { offset.x + c * CELL, offset.y + r * CELL }, 0.f, true };
                    matrix[r][c] = g;
                    ++moved;
                }
            }
        }
    }
    return moved;
}

// INTERCAMBIO
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

// DIBUJO
void Board::drawBoard(RenderWindow& window) {
    RectangleShape cell({ (float)CELL - 6, (float)CELL - 6 });
    cell.setFillColor(Color(255, 255, 255, 215));
    cell.setOutlineColor(Color(220, 220, 220));
    cell.setOutlineThickness(3);

    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c) {
            cell.setPosition(offset.x + c * CELL + 3, offset.y + r * CELL + 3);
            window.draw(cell);
        }

    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c)
            if (matrix[r][c] && !matrix[r][c]->isEmpty())
                matrix[r][c]->draw(window);

    drawExplosions(window);
    drawFlashes(window);
}

// FUNCIONES AUXILIARES
bool Board::markForClear(int r, int c) {
    if (isValid(r, c) && matrix[r][c]) {
        string tipo = matrix[r][c]->getTipoGem();

        if (pTotoroCount && tipo == "Totoro" && tipo.find("Especial") == string::npos)
            (*pTotoroCount)++;
        if (pPonyoCount && tipo == "Ponyo" && tipo.find("Especial") == string::npos)
            (*pPonyoCount)++;
        if (pIceCount && tipo == "Ice")
            (*pIceCount)++;

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
            return true;
        }
    }
    return false;
}

void Board::clearMarked() {}

int Board::swapCells(int r1, int c1, int r2, int c2) {
    if (!isValid(r1, c1) || !isValid(r2, c2)) return 0;
    std::swap(matrix[r1][c1], matrix[r2][c2]);
    if (matrix[r1][c1]) matrix[r1][c1]->setGrid(r1, c1, CELL, offset.x, offset.y);
    if (matrix[r2][c2]) matrix[r2][c2]->setGrid(r2, c2, CELL, offset.x, offset.y);
    return 1;
}

bool Board::screenToCell(int x, int y, int& fila, int& col) {
    x -= (int)offset.x;
    y -= (int)offset.y;
    if (x < 0 || y < 0) return false;
    col = x / CELL;
    fila = y / CELL;
    return isValid(fila, col);
}

void Board::drawSelection(RenderWindow& window, int fila, int columna) {
    if (!isValid(fila, columna)) return;
    RectangleShape rect({ (float)CELL,(float)CELL });
    rect.setPosition(offset.x + columna * CELL, offset.y + fila * CELL);
    rect.setFillColor(Color::Transparent);
    rect.setOutlineColor(Color::Yellow);
    rect.setOutlineThickness(5);
    window.draw(rect);
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
