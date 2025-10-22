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
using namespace std;
using namespace sf;

// Constructor del tablero: se ejecuta al crear un objeto Board
Board::Board() {
    // Inicializa el generador de números aleatorios con la hora actual
    srand(static_cast<unsigned>(time(nullptr)));
    // Llena el tablero con gemas
    fillBoard();
}

// Destructor del tablero: se ejecuta al destruir un objeto Board
Board::~Board() {
    // Libera la memoria de todas las gemas del tablero
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c)
            delete matrix[r][c];
}

// CONFIGURACIÓN DE TABLERO

// Establece la posición del tablero en la ventana
void Board::setOffset(float x, float y) {
    offset = { x, y };
    // Ajusta la posición de cada gema según la nueva ubicación del tablero
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c)
            if (matrix[r][c])
                matrix[r][c]->setGrid(r, c, CELL, offset.x, offset.y);
}

// Centra el tablero en la ventana, dejando espacio para la barra de HUD (interfaz superior)
void Board::centerInWindow(int winW, int winH, int hud) {
    const int boardW = N * CELL;  // Ancho total del tablero
    const int boardH = N * CELL;  // Alto total del tablero
    // Calcula la posición X centrada
    float offX = (winW - boardW) * 0.5f;
    // Calcula la posición Y centrada, pero debajo del HUD
    float offY = hud + (winH - hud - boardH) * 0.5f;
    setOffset(offX, offY);  // Aplica la nueva posición
}

// Verifica si una posición (fila, columna) está dentro del tablero
bool Board::isValid(int r, int c) const {
    return (r >= 0 && r < N && c >= 0 && c < N);
}

// Coloca una gema en una celda específica del tablero
void Board::setGem(int r, int c, Gem* g) {
    if (!isValid(r, c)) return;  // Si la posición no es válida, no hace nada
    matrix[r][c] = g;  // Guarda el puntero a la gema en la matriz
}

// Elimina la gema de una celda (libera memoria y pone nullptr)
void Board::clearCell(int r, int c) {
    if (!isValid(r, c)) return;
    delete matrix[r][c];        // Libera la memoria de la gema
    matrix[r][c] = nullptr;     // Marca la celda como vacía
}

// Devuelve la gema que está en una posición dada (o nullptr si está vacía o fuera del tablero)
Gem* Board::getGem(int r, int c) const {
    return isValid(r, c) ? matrix[r][c] : nullptr;
}

// LLENAR TABLERO

// Llena todo el tablero con gemas al azar, evitando que se formen tríos al inicio
void Board::fillBoard() {
    // Lista de tipos de gemas normales (sin incluir Ice al principio del chequeo)
    const string tipos[6] = { "Totoro","Ponyo","Parti","Gato","Galleta","Ice" };

    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            string tipo;
            bool ok;
            // Repite hasta encontrar un tipo que no forme un trío con las gemas anteriores
            do {
                ok = true;
                tipo = tipos[rand() % 6];  // Elige un tipo al azar

                // Solo revisa tríos si no es una gema de hielo ("Ice")
                if (tipo != "Ice") {
                    // Revisa si hay dos gemas iguales a la izquierda
                    if (c >= 2 && matrix[r][c - 1] && matrix[r][c - 2] &&
                        matrix[r][c - 1]->getTipoGem() == tipo &&
                        matrix[r][c - 2]->getTipoGem() == tipo) ok = false;
                    // Revisa si hay dos gemas iguales arriba
                    if (r >= 2 && matrix[r - 1][c] && matrix[r - 2][c] &&
                        matrix[r - 1][c]->getTipoGem() == tipo &&
                        matrix[r - 2][c]->getTipoGem() == tipo) ok = false;
                }
            } while (!ok);  // Si no es válido, prueba con otro tipo

            // Crea la gema según el tipo elegido
            Gem* g = nullptr;
            if (tipo == "Totoro") g = new TotoroGem();
            else if (tipo == "Ponyo") g = new PonyoGem();
            else if (tipo == "Parti") g = new PartiGem();
            else if (tipo == "Gato") g = new GatoGem();
            else if (tipo == "Galleta") g = new GalletaGem();
            else if (tipo == "Ice") g = new IceGem();
            else g = new NormalGem();  // Por si acaso (no debería pasar)

            // Guarda el tipo en la gema y la coloca en su posición visual
            g->setTipoGem(tipo);
            g->setGrid(r, c, CELL, offset.x, offset.y);
            setGem(r, c, g);  // La guarda en la matriz del tablero
        }
    }
}

// ANIMACIONES

// Verifica si hay alguna animación activa en el tablero
bool Board::anyAnimationActive() const {
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c)
            if (animMatrix[r][c].active)
                return true;
    return false;
}

// Actualiza el progreso de las animaciones de caída de gemas
void Board::updateAnimations(float dt) {
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (!animMatrix[r][c].active) continue;  // Salta si no hay animación
            auto& anim = animMatrix[r][c];
            anim.progress += dt * fallSpeed;  // Avanza la animación

            // Si ya terminó, la detiene
            if (anim.progress >= 1.f) {
                anim.progress = 1.f;
                anim.active = false;
            }

            // Mueve la gema visualmente según el progreso
            if (matrix[r][c]) {
                Vector2f pos = anim.startPos + (anim.endPos - anim.startPos) * anim.progress;
                matrix[r][c]->getSprite().setPosition(pos);
            }
        }
    }
}

// Actualiza las explosiones (efectos visuales al eliminar gemas)
void Board::updateExplosions(float dt) {
    for (auto& e : explosions) {
        if (!e.active) continue;
        e.lifetime -= dt;           // Reduce el tiempo de vida
        e.radius += 260.f * dt;     // Aumenta el radio de la explosión
        if (e.lifetime <= 0.f) e.active = false;  // La desactiva si se acabó el tiempo
    }
}

// Dibuja las explosiones en la ventana
void Board::drawExplosions(RenderWindow& window) {
    for (auto& e : explosions) {
        if (!e.active) continue;
        CircleShape circle(e.radius);             // Crea un círculo
        circle.setOrigin(e.radius, e.radius);     // Centra el origen
        circle.setPosition(e.position);           // Lo coloca en la posición de la explosión
        circle.setFillColor(Color(255, 200, 0, 60));         // Color amarillo transparente
        circle.setOutlineColor(Color(255, 140, 0, 180));     // Borde naranja más opaco
        circle.setOutlineThickness(4);
        window.draw(circle);
    }
}

// Actualiza los destellos (efectos de luz breves)
void Board::updateFlashes(float dt) {
    for (auto& f : flashes) {
        if (!f.active) continue;
        f.timer += dt;                    // Avanza el temporizador
        if (f.timer >= 0.25f) f.active = false;  // Desaparece después de 0.25 segundos
    }
}

// Dibuja los destellos en la ventana
void Board::drawFlashes(RenderWindow& window) {
    for (auto& f : flashes) {
        if (!f.active) continue;
        float progress = f.timer / 0.25f;         // Cuánto ha avanzado (0 a 1)
        float alpha = 255 * (1.f - progress);     // Se vuelve más transparente con el tiempo
        float radius = 20.f + 40.f * progress;    // Crece mientras dura

        CircleShape flash(radius);
        flash.setOrigin(radius, radius);
        flash.setPosition(f.position);
        flash.setFillColor(Color(255, 255, 180, (int)alpha));  // Amarillo claro
        window.draw(flash);
    }
}

// Crea una gema especial en una posición dada, basada en un tipo normal
void Board::createSpecialAt(int r, int c, const std::string& tipoBase) {
    // Elimina la gema que ya estaba ahí
    delete matrix[r][c];
    Gem* s = nullptr;

    // Crea una nueva gema del mismo tipo base
    if (tipoBase == "Totoro")       s = new TotoroGem();
    else if (tipoBase == "Ponyo")   s = new PonyoGem();
    else if (tipoBase == "Parti")   s = new PartiGem();
    else if (tipoBase == "Gato")    s = new GatoGem();
    else if (tipoBase == "Galleta") s = new GalletaGem();
    else                            s = new NormalGem();

    // Le pone un nombre especial para identificarla
    s->setTipoGem(tipoBase + "Especial");
    // La coloca en la cuadrícula
    s->setGrid(r, c, CELL, offset.x, offset.y);
    matrix[r][c] = s;

    // (Opcional) Aquí podrías añadir un destello visual para indicar que nació una gema especial
}

// MATCH Y LIMPIEZA

// Busca combinaciones de 3 o más gemas iguales y las elimina
int Board::findAndClearMatches() {
    // Matriz que marca qué celdas se van a eliminar
    bool mark[N][N] = { false };
    int cleared = 0;  // Cuenta cuántas gemas se eliminan

    // Revisa combinaciones HORIZONTALES 
    for (int r = 0; r < N; ++r) {
        int count = 1;  // Cuenta gemas consecutivas del mismo tipo
        for (int c = 1; c <= N; ++c) {
            // Compara con la gema anterior
            bool same = (c < N && matrix[r][c] && matrix[r][c - 1] &&
                matrix[r][c]->getTipoGem() == matrix[r][c - 1]->getTipoGem());
            if (same) {
                ++count;
            }
            else {
                // Si hay 3 o más, marca esas celdas
                if (count >= 3 && matrix[r][c - 1]) {
                    for (int k = c - count; k < c; ++k) mark[r][k] = true;

                    // Si son 4 o más, crea una gema especial en el centro (pero no con "Ice")
                    if (count >= 4) {
                        int center = c - count + count / 2;
                        if (matrix[r][center]) {
                            std::string base = matrix[r][center]->getTipoGem();
                            // Si ya era especial, quita la palabra "Especial" para saber el tipo base
                            auto pos = base.find("Especial");
                            if (pos != std::string::npos) base = base.substr(0, pos);

                            if (base != "Ice") {
                                // No elimina la gema del centro, sino que la convierte en especial
                                mark[r][center] = false;
                                createSpecialAt(r, center, base);
                            }
                        }
                    }
                }
                count = 1;  // Reinicia el contador
            }
        }
    }

    // Revisa combinaciones VERTICALES 
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

    //  Elimina todas las gemas marcadas 
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (mark[r][c] && matrix[r][c]) {
                // Si la gema tiene un efecto especial, lo activa
                matrix[r][c]->onMatch(*this, r, c);

                // Crea un efecto visual de explosión
                Explosion e;
                e.position = sf::Vector2f(offset.x + c * CELL + CELL * 0.5f,
                    offset.y + r * CELL + CELL * 0.5f);
                e.radius = 0.f;
                e.lifetime = 0.5f;
                e.active = true;
                explosions.push_back(e);

                // Marca la gema para ser eliminada (y actualiza contadores si es necesario)
                markForClear(r, c);
                ++cleared;
            }
        }
    }

    return cleared;  // Devuelve cuántas gemas se eliminaron
}


// GRAVEDAD + RELLENO (con animación)

// Hace que las gemas caigan por gravedad y rellena los huecos con nuevas gemas
int Board::applyGravityAndRefill() {
    int moved = 0;
    // Tipos de gemas que pueden aparecer al rellenar (sin "Ice")
    const string tipos[5] = { "Totoro","Ponyo","Parti","Gato","Galleta" };

    // Recorre cada columna de abajo hacia arriba
    for (int c = 0; c < N; ++c) {
        for (int r = N - 1; r >= 0; --r) {
            // Si la celda está vacía
            if (matrix[r][c] == nullptr) {
                // Busca la primera gema encima que no esté vacía
                int k = r - 1;
                while (k >= 0 && matrix[k][c] == nullptr) --k;

                if (k >= 0) {
                    // Mueve esa gema hacia abajo
                    matrix[r][c] = matrix[k][c];
                    matrix[k][c] = nullptr;

                    // Prepara la animación de caída
                    Vector2f start = { offset.x + c * CELL, offset.y + k * CELL };
                    Vector2f end = { offset.x + c * CELL, offset.y + r * CELL };
                    matrix[r][c]->setGrid(r, c, CELL, offset.x, offset.y);
                    matrix[r][c]->getSprite().setPosition(start);
                    animMatrix[r][c] = { start, end, 0.f, true };
                    ++moved;
                }
                else {
                    // Si no hay gemas arriba, crea una nueva desde arriba de la pantalla
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

// INTERCAMBIO DE GEMAS

// Verifica si un intercambio entre dos gemas produciría una combinación válida
bool Board::isSwapValid(int r1, int c1, int r2, int c2) {
    // Comprueba que ambas posiciones sean válidas y tengan gemas
    if (!isValid(r1, c1) || !isValid(r2, c2)) return false;
    Gem* g1 = matrix[r1][c1];
    Gem* g2 = matrix[r2][c2];
    if (!g1 || !g2) return false;

    // No se puede intercambiar con gemas de hielo
    if (dynamic_cast<IceGem*>(g1) || dynamic_cast<IceGem*>(g2))
        return false;

    // Simula el intercambio
    std::swap(matrix[r1][c1], matrix[r2][c2]);
    bool found = false;

    // Revisa filas afectadas (las de las dos gemas)
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

    // Si no encontró en filas, revisa columnas
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

    // Deshace el intercambio simulado
    std::swap(matrix[r1][c1], matrix[r2][c2]);
    return found;  // Devuelve si el intercambio sería válido
}

// DIBUJO GENERAL

// Dibuja todo el tablero: fondo, gemas, explosiones y destellos
void Board::drawBoard(RenderWindow& window) {
    // Dibuja las celdas del tablero (fondo de cada casilla)
    RectangleShape cell({ (float)CELL - 6, (float)CELL - 6 });
    cell.setFillColor(Color(255, 255, 255, 215));
    cell.setOutlineColor(Color(220, 220, 220));
    cell.setOutlineThickness(3);

    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c) {
            cell.setPosition(offset.x + c * CELL + 3, offset.y + r * CELL + 3);
            window.draw(cell);
        }

    // Dibuja cada gema que no esté vacía
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c)
            if (matrix[r][c] && !matrix[r][c]->isEmpty())
                matrix[r][c]->draw(window);

    // Dibuja efectos visuales
    drawExplosions(window);
    drawFlashes(window);
}


// FUNCIONES AUXILIARES

// Marca una gema para ser eliminada y actualiza contadores si es necesario
bool Board::markForClear(int r, int c) {
    if (isValid(r, c) && matrix[r][c]) {
        string tipo = matrix[r][c]->getTipoGem();

        // Si hay punteros a contadores, los actualiza (solo para gemas normales, no especiales)
        if (pTotoroCount && tipo == "Totoro" && tipo.find("Especial") == string::npos)
            (*pTotoroCount)++;
        if (pPonyoCount && tipo == "Ponyo" && tipo.find("Especial") == string::npos)
            (*pPonyoCount)++;
        if (pIceCount && tipo == "Ice")
            (*pIceCount)++;

        // Si es una gema de hielo, recibe un golpe
        IceGem* ice = dynamic_cast<IceGem*>(matrix[r][c]);
        if (ice && !ice->isBroken()) {
            ice->receiveHit();
            if (ice->isBroken()) {
                delete matrix[r][c];
                matrix[r][c] = nullptr;
                return true;
            }
            return false;  // No se elimina aún si no está rota
        }
        else {
            // Elimina la gema normal
            delete matrix[r][c];
            matrix[r][c] = nullptr;
            return true;
        }
    }
    return false;
}

// Función de compatibilidad (no hace nada, pero existe por si se usa en otro lado)
void Board::clearMarked() {
    // placeholder para compatibilidad
}

// Intercambia dos celdas del tablero (sin validación)
int Board::swapCells(int r1, int c1, int r2, int c2) {
    if (!isValid(r1, c1) || !isValid(r2, c2)) return 0;
    std::swap(matrix[r1][c1], matrix[r2][c2]);
    // Actualiza la posición visual de ambas gemas
    if (matrix[r1][c1]) matrix[r1][c1]->setGrid(r1, c1, CELL, offset.x, offset.y);
    if (matrix[r2][c2]) matrix[r2][c2]->setGrid(r2, c2, CELL, offset.x, offset.y);
    return 1;
}

// Convierte coordenadas de la pantalla a una celda del tablero
bool Board::screenToCell(int x, int y, int& fila, int& col) {
    x -= static_cast<int>(offset.x);
    y -= static_cast<int>(offset.y);
    if (x < 0 || y < 0) return false;
    col = x / CELL;
    fila = y / CELL;
    return isValid(fila, col);
}

// Dibuja un borde amarillo alrededor de una celda (para resaltar selección)
void Board::drawSelection(sf::RenderWindow& window, int fila, int columna) {
    if (!isValid(fila, columna)) return;
    sf::RectangleShape rect({ (float)CELL,(float)CELL });
    rect.setPosition(offset.x + columna * CELL, offset.y + fila * CELL);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Yellow);
    rect.setOutlineThickness(5);
    window.draw(rect);
}

// Coloca una gema de hielo en una posición aleatoria del tablero
void Board::placeRandomIce() {
    for (int tries = 0; tries < 50; ++tries) {
        int r = std::rand() % N;
        int c = std::rand() % N;
        // Solo coloca si hay una gema normal (no vacía y no ya de hielo)
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