#include "Game.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>  // Solo en Windows: para mostrar ventanas de error
#endif
using namespace sf;
using namespace std;

// Constructor y destructor de la clase Game (vacíos por ahora)
Game::Game() {}
Game::~Game() {}

// Actualiza los textos del HUD (puntuación, movimientos, nivel y objetivo)
void Game::updateHUD() {
    if (!hudOk) return;  // Si el HUD no está listo, no hace nada

    // Muestra la puntuación actual
    scoreText.setString("Puntos: " + std::to_string(punctuation));
    // Muestra cuántos movimientos quedan
    movesText.setString("Movimientos: " + std::to_string(counter));
    // Muestra el nivel actual
    levelText.setString("Nivel: " + std::to_string(currentLevel));

    // Define el texto del objetivo según el nivel
    std::string objText;
    if (currentLevel == 1)
        objText = "Objetivo: " + std::to_string(totoroCleared) + "/30 Totoro";
    else if (currentLevel == 2)
        objText = "Objetivo: " + std::to_string(iceCleared) + "/10 Hielo";
    else if (currentLevel == 3)
        objText = "Objetivo: " + std::to_string(ponyoCleared) + "/20 Ponyo";

    objectiveText.setString(objText);  // Lo asigna al texto del HUD
}

// Ajusta un sprite para que cubra toda la ventana (útil para fondos)
void Game::coverSpriteToWindow(Sprite& spr, const Vector2u& win) {
    FloatRect b = spr.getLocalBounds();  // Tamaño original del sprite
    // Calcula el factor de escala necesario para cubrir la ventana
    float s = std::max((float)win.x / b.width, (float)win.y / b.height);
    spr.setScale(s, s);  // Aplica la escala
    // Centra el sprite en la ventana
    spr.setPosition((win.x - b.width * s) * 0.5f, (win.y - b.height * s) * 0.5f);
}

// Inicia un nuevo juego desde el nivel 1
void Game::startGame() {
    punctuation = 0;        // Reinicia la puntuación
    counter = 20;           // Da 20 movimientos
    currentLevel = 1;       // Empieza en el nivel 1
    movesSinceIce = 0;      // Contador para colocar hielo

    // Reinicia los contadores de gemas recolectadas
    totoroCleared = 0;
    iceCleared = 0;
    ponyoCleared = 0;
    // Le dice al tablero dónde guardar los contadores
    board.setCounters(&totoroCleared, &iceCleared, &ponyoCleared);

    gameOver = false;
    gameState = GameState::Playing;  // Estado: jugando

    // Elimina cualquier combinación que ya exista al inicio
    int cleared = board.findAndClearMatches();
    int safety = 0;
    while (cleared > 0 && safety++ < 12) {  // Evita bucles infinitos
        punctuation += cleared * 10;        // Suma puntos
        board.applyGravityAndRefill();      // Hace caer gemas y rellena
        cleared = board.findAndClearMatches();  // Revisa de nuevo
    }
    updateHUD();  // Actualiza los textos en pantalla
}

// Pasa al siguiente nivel con una transición de fundido a negro
void Game::nextLevel() {
    // Crea una ventana temporal solo para la animación de transición
    sf::Clock clock;
    float fade = 0.f;

    sf::RenderWindow temp(VideoMode(880, 930), "Match Studio Ghibli");
    temp.setFramerateLimit(60);
    sf::RectangleShape overlay(Vector2f(temp.getSize().x, temp.getSize().y));
    overlay.setFillColor(Color(0, 0, 0, 0));  // Negro transparente

    // Fundido a negro (aumenta opacidad)
    while (fade < 255) {
        fade += 400 * clock.restart().asSeconds();
        overlay.setFillColor(Color(0, 0, 0, (int)fade));
        temp.clear(Color::Black);
        temp.draw(overlay);
        temp.display();
    }

    // Si ya completó el nivel 3, gana el juego
    if (currentLevel >= 3) {
        state = 2;
        gameOver = true;
        gameState = GameState::WinAll;
        return;
    }

    // Prepara el siguiente nivel
    ++currentLevel;
    counter = 20;
    movesSinceIce = 0;

    totoroCleared = 0;
    iceCleared = 0;
    ponyoCleared = 0;
    board.setCounters(&totoroCleared, &iceCleared, &ponyoCleared);

    board.fillBoard();  // Llena el tablero de nuevo

    // Elimina combinaciones iniciales (como en startGame)
    int cleared = board.findAndClearMatches();
    int safety = 0;
    while (cleared > 0 && safety++ < 12) {
        punctuation += cleared * 10;
        board.applyGravityAndRefill();
        cleared = board.findAndClearMatches();
    }

    gameState = GameState::Playing;
    updateHUD();
}

// Revisa si el jugador cumplió el objetivo del nivel actual
void Game::checkLevelAdvance() {
    bool done = false;
    if (currentLevel == 1) done = (totoroCleared >= TOTORO_GOAL);   // 30 Totoro
    else if (currentLevel == 2) done = (iceCleared >= ICE_GOAL);     // 10 Hielo
    else if (currentLevel == 3) done = (ponyoCleared >= PONYO_GOAL); // 20 Ponyo

    if (done) nextLevel();  // Si cumplió, pasa al siguiente nivel
}

// Procesa una ronda de eliminaciones y caídas (cascadas)
void Game::processCascadesOnce() {
    int cleared = board.findAndClearMatches();
    if (cleared > 0) {
        punctuation += cleared * 10;     // Suma puntos
        board.applyGravityAndRefill();   // Hace caer gemas
        updateHUD();                     // Actualiza el HUD
        checkLevelAdvance();             // Revisa si completó el nivel
    }
}

// Función principal: ejecuta todo el juego
void Game::run() {
    try {
        const int HUD_H = 50;  // Altura de la barra superior (HUD)
        RenderWindow window(VideoMode(880, 930), "Match Studio Ghibli");
        window.setFramerateLimit(60);  // Máximo 60 cuadros por segundo

        // Centra el tablero en la ventana, dejando espacio para el HUD
        board.centerInWindow(window.getSize().x, window.getSize().y, HUD_H);

        // Carga la fuente (Arial). Si falla, lanza un error
        if (!font.loadFromFile("arial.ttf") && !font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
            throw std::string("Error: No se pudo cargar la fuente 'arial.ttf'");

        // Carga las imágenes de fondo
        if (!bgTexture.loadFromFile("assets/FondoM.png"))
            throw std::string("Error: No se pudo cargar 'assets/FondoM.png'");
        if (!bgFinalTexture.loadFromFile("assets/Fondo2.png"))
            throw std::string("Error: No se pudo cargar 'assets/Fondo2.png'");

        hudOk = true;  // Indica que el HUD ya se puede usar

        // Configura los textos del HUD
        scoreText.setFont(font);  scoreText.setCharacterSize(22); scoreText.setFillColor(Color::White);
        movesText.setFont(font);  movesText.setCharacterSize(22); movesText.setFillColor(Color::White);
        levelText.setFont(font);  levelText.setCharacterSize(22); levelText.setFillColor(Color::White);
        overText.setFont(font);   overText.setCharacterSize(40);  overText.setFillColor(Color(0, 100, 0));
        objectiveText.setFont(font); objectiveText.setCharacterSize(20); objectiveText.setFillColor(Color::White);

        // Crea los botones del menú
        playButton.setSize({ 220.f, 90.f }); playButton.setFillColor(Color(0, 100, 0));
        restartButton.setSize({ 220.f, 90.f }); restartButton.setFillColor(Color(0, 100, 0));
        exitButton.setSize({ 220.f, 90.f }); exitButton.setFillColor(Color(0, 100, 0));

        // Texto de los botones
        playText.setFont(font); playText.setCharacterSize(32); playText.setFillColor(Color::White); playText.setString("Jugar");
        restartText.setFont(font); restartText.setCharacterSize(28); restartText.setFillColor(Color::White); restartText.setString("Reiniciar");
        exitText.setFont(font); exitText.setCharacterSize(28); exitText.setFillColor(Color::White); exitText.setString("Salir");

        // Configura los sprites de fondo
        bgSprite.setTexture(bgTexture);
        coverSpriteToWindow(bgSprite, window.getSize());
        bgFinalSprite.setTexture(bgFinalTexture);
        coverSpriteToWindow(bgFinalSprite, window.getSize());

        // Funciones lambda para centrar texto y botones
        auto centerText = [](Text& t, const Vector2f& p) {
            FloatRect b = t.getLocalBounds();
            t.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
            t.setPosition(p);
            };
        auto centerRect = [](RectangleShape& r, const Vector2f& p) {
            r.setOrigin(r.getSize() * 0.5f);
            r.setPosition(p);
            };

        // Coloca los botones en posiciones específicas
        centerRect(playButton, { (float)window.getSize().x * 0.5f, (float)window.getSize().y * 0.75f });
        centerText(playText, playButton.getPosition());
        centerRect(restartButton, { (float)window.getSize().x * 0.5f, (float)window.getSize().y * 0.58f });
        centerText(restartText, restartButton.getPosition());
        centerRect(exitButton, { (float)window.getSize().x * 0.5f, (float)window.getSize().y * 0.70f });
        centerText(exitText, exitButton.getPosition());

        updateHUD();  // Actualiza el HUD por primera vez
        Clock clock;

        selR = selC = -1;  // No hay celda seleccionada al inicio

        //  LOOP PRINCIPAL DEL JUEGO 
        while (window.isOpen()) {
            float dt = clock.restart().asSeconds();  // Tiempo desde el último cuadro

            // Procesa eventos (clics, cerrar ventana, etc.)
            Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == Event::Closed) window.close();

                // Estado 0: Menú principal
                if (state == 0) {
                    if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left) {
                        // Si se hace clic en "Jugar", inicia el juego
                        if (playButton.getGlobalBounds().contains((float)ev.mouseButton.x, (float)ev.mouseButton.y)) {
                            state = 1;
                            startGame();
                        }
                    }
                }
                // Estado 1: Jugando
                else if (state == 1 && !gameOver && gameState == GameState::Playing) {
                    if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left) {
                        int r, c;
                        // Convierte la posición del clic a una celda del tablero
                        if (board.screenToCell(ev.mouseButton.x, ev.mouseButton.y, r, c)) {
                            if (selR == -1) {
                                // Primera selección: marca la celda
                                Gem* g = board.getGem(r, c);
                                if (g) { selR = r; selC = c; }
                            }
                            else {
                                // Segunda selección: intenta intercambiar o usar gema especial
                                Gem* g1 = board.getGem(selR, selC);
                                Gem* g2 = board.getGem(r, c);

                                // Verifica si alguna de las dos es una gema especial no activada
                                bool special1 = (g1 && g1->getIsSpecial() && !g1->getIsActivated());
                                bool special2 = (g2 && g2->getIsSpecial() && !g2->getIsActivated());

                                if (special1 || special2) {
                                    // Intercambia las gemas
                                    board.swapCells(selR, selC, r, c);

                                    // Activa el efecto de la gema especial
                                    Gem* g1_after = board.getGem(r, c);
                                    Gem* g2_after = board.getGem(selR, selC);

                                    if (special1 && g1_after) { g1_after->onMatch(board, r, c); punctuation += 50; }
                                    if (special2 && g2_after) { g2_after->onMatch(board, selR, selC); punctuation += 50; }

                                    // Aplica gravedad y rellena
                                    board.applyGravityAndRefill();
                                    ++movesSinceIce; --counter;
                                    // Cada ciertos movimientos, coloca una gema de hielo
                                    if (movesSinceIce >= ICE_EVERY) { board.placeRandomIce(); movesSinceIce = 0; }

                                    // Procesa todas las eliminaciones en cadena
                                    int cleared = 0, safety = 0;
                                    do {
                                        cleared = board.findAndClearMatches();
                                        if (cleared > 0) { punctuation += cleared * 10; board.applyGravityAndRefill(); }
                                    } while (cleared > 0 && ++safety < 12);

                                    checkLevelAdvance();
                                    // Si se acaban los movimientos, termina el juego
                                    if (!gameOver && counter <= 0) { gameOver = true; state = 2; }
                                    updateHUD();
                                }
                                else if (board.isSwapValid(selR, selC, r, c)) {
                                    // Si es un intercambio válido normal
                                    board.swapCells(selR, selC, r, c);
                                    ++movesSinceIce; --counter;
                                    if (movesSinceIce >= ICE_EVERY) { board.placeRandomIce(); movesSinceIce = 0; }

                                    // Procesa eliminaciones en cadena
                                    int cleared = 0, safety = 0;
                                    do {
                                        cleared = board.findAndClearMatches();
                                        if (cleared > 0) { punctuation += cleared * 10; board.applyGravityAndRefill(); }
                                    } while (cleared > 0 && ++safety < 12);

                                    checkLevelAdvance();
                                    if (!gameOver && counter <= 0) { gameOver = true; state = 2; }
                                    updateHUD();
                                }
                                // Desmarca la selección
                                selR = selC = -1;
                            }
                        }
                    }
                }
                // Estado 2: Pantalla final (ganó o perdió)
                else if (state == 2) {
                    if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left) {
                        Vector2f m{ (float)ev.mouseButton.x, (float)ev.mouseButton.y };
                        // Botón "Reiniciar": vuelve al menú
                        if (restartButton.getGlobalBounds().contains(m)) {
                            state = 0;
                            punctuation = 0; counter = 20; gameOver = false;
                            currentLevel = 1; movesSinceIce = 0;
                            updateHUD();
                        }
                        // Botón "Salir": cierra el juego
                        else if (exitButton.getGlobalBounds().contains(m)) {
                            window.close();
                        }
                    }
                }
            }

            //  ACTUALIZACIÓN DE ANIMACIONES 
            board.updateAnimations(dt);
            board.updateExplosions(dt);
            board.updateFlashes(dt);

            //  DIBUJO EN PANTALLA 
            window.clear();

            if (state == 0) {
                // Menú principal: fondo + botón "Jugar"
                window.draw(bgSprite);
                window.draw(playButton); window.draw(playText);
            }
            else if (state == 1) {
                // Jugando: dibuja el HUD y el tablero
                RectangleShape hudBg({ (float)window.getSize().x, (float)HUD_H });
                hudBg.setFillColor(Color(0, 100, 0, 230));  // Verde oscuro semitransparente
                window.draw(hudBg);

                // Posiciona los textos del HUD
                scoreText.setPosition(20, 14);
                objectiveText.setPosition(20, 40);
                movesText.setPosition((float)window.getSize().x - 240.f, 14.f);
                levelText.setPosition((float)window.getSize().x * 0.5f - 60.f, 14.f);

                window.draw(scoreText);
                window.draw(objectiveText);
                window.draw(movesText);
                window.draw(levelText);

                board.drawBoard(window);  // Dibuja el tablero

                // Si hay una celda seleccionada, la resalta
                if (gameState == GameState::Playing && selR != -1)
                    board.drawSelection(window, selR, selC);
            }
            else if (state == 2) {
                // Pantalla final: fondo + mensaje + botones
                window.draw(bgFinalSprite);

                string msg = (gameState == GameState::WinAll)
                    ? "¡Ganaste todos los niveles!"
                    : ("Puntos: " + std::to_string(punctuation));

                overText.setString(msg);
                FloatRect b = overText.getLocalBounds();
                overText.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
                overText.setPosition((float)window.getSize().x * 0.5f, (float)window.getSize().y * 0.40f);
                window.draw(overText);

                window.draw(restartButton); window.draw(restartText);
                window.draw(exitButton);    window.draw(exitText);
            }

            window.display();  // Muestra todo en pantalla
        }

    }
    // Manejo de errores: muestra mensajes en consola y en ventana (solo en Windows)
    catch (const char* msg) {
        cerr << "[❌ ERROR] " << msg << endl;
#ifdef _WIN32
        MessageBoxA(nullptr, msg, "Error Crítico", MB_ICONERROR);
#endif
    }
    catch (const std::string& msg) {
        cerr << "[❌ ERROR] " << msg << endl;
#ifdef _WIN32
        MessageBoxA(nullptr, msg.c_str(), "Error Crítico", MB_ICONERROR);
#endif
    }
    catch (...) {
        cerr << "[❌ ERROR] Excepción desconocida" << endl;
#ifdef _WIN32
        MessageBoxA(nullptr, "Error desconocido. El juego no puede continuar.", "Error", MB_ICONERROR);
#endif
    }
}