#include "Game.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
<<<<<<< HEAD
#include "Ranking.h"
#include "LevelData.h"
#include <fstream>
=======
#include <fstream>
#include <sstream>
>>>>>>> develop

using namespace sf;
using namespace std;

static void centerText(sf::Text& t, float x, float y) {
    auto b = t.getLocalBounds();
    t.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
    t.setPosition(x, y);
}

Game::Game() : progress("progress.txt") {}
Game::~Game() {}

void Game::updateHUD() {
    if (!hudOk) return;
    scoreText.setString("Puntos: " + std::to_string(punctuation));
    movesText.setString("Movimientos: " + std::to_string(counter));
    levelText.setString("Nivel: " + std::to_string(currentLevel));

    std::string objText;
    if (currentLevel == 1)
        objText = "Objetivo: " + std::to_string(totoroCleared) + "/20 Totoro";
    else if (currentLevel == 2)
        objText = "Objetivo: " + std::to_string(iceCleared) + "/10 Hielo";
    else if (currentLevel == 3)
        objText = "Objetivo: " + std::to_string(ponyoCleared) + "/30 Ponyo";

    objectiveText.setString(objText);
}

void Game::coverSpriteToWindow(Sprite& spr, const Vector2u& win) {
    FloatRect b = spr.getLocalBounds();
    float s = std::max((float)win.x / b.width, (float)win.y / b.height);
    spr.setScale(s, s);
    spr.setPosition((win.x - b.width * s) * 0.5f, (win.y - b.height * s) * 0.5f);
}

void Game::startGame() {
    punctuation = 0;
    counter = 20;
    movesSinceIce = 0;

<<<<<<< HEAD
    //  CARGA DE NIVEL DESDE ARCHIVO
    LevelData data;
    std::string filename = "levels/nivel" + std::to_string(currentLevel) + ".txt";
    if (data.loadFromFile(filename)) {
        // Aplica los valores cargados del archivo
        counter = data.movimientos;
        const_cast<int&>(TOTORO_GOAL) = data.totoroGoal;
        const_cast<int&>(ICE_GOAL) = data.iceGoal;
        const_cast<int&>(PONYO_GOAL) = data.ponyoGoal;

        // Cambia el fondo del juego si el archivo tiene uno distinto
        if (!data.fondo.empty()) {
            bgTexture.loadFromFile(data.fondo);
            bgSprite.setTexture(bgTexture);
        }
    }

=======
    // Mantener el currentLevel que ya definimos (desde menú de niveles)
>>>>>>> develop
    totoroCleared = 0;
    iceCleared = 0;
    ponyoCleared = 0;
    board.setCounters(&totoroCleared, &iceCleared, &ponyoCleared);

    gameOver = false;
    gameState = GameState::Playing;

    // Limpiar combinaciones iniciales
    int cleared = board.findAndClearMatches();
    int safety = 0;
    while (cleared > 0 && safety++ < 12) {
        punctuation += cleared * 10;
        board.applyGravityAndRefill();
        cleared = board.findAndClearMatches();
    }
    updateHUD();
}
<<<<<<< HEAD
void Game::nextLevel() {
    sf::Clock clock;
    float fade = 0.f;
=======

void Game::showLevelTransition(sf::RenderWindow& window, int nextLvl) {
    sf::RectangleShape fadeRect(sf::Vector2f(window.getSize()));
    fadeRect.setFillColor(sf::Color(255, 240, 220, 0)); // Beige suave
>>>>>>> develop

    // Usar la misma font ya cargada
    sf::Text text("Cargando nivel " + std::to_string(nextLvl), font, 60);
    text.setFillColor(sf::Color(34, 102, 34)); // Verde Ghibli
    auto tb = text.getLocalBounds();
    text.setOrigin(tb.width / 2.f, tb.height / 2.f);
    text.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

    // Fundido de entrada
    for (int alpha = 0; alpha <= 200; alpha += 10) {
        fadeRect.setFillColor(sf::Color(255, 240, 220, alpha));
        window.clear();
        window.draw(bgSprite);
        window.draw(fadeRect);
        if (alpha > 90) window.draw(text);
        window.display();
        sf::sleep(sf::milliseconds(16));
    }

    // Mantener un instante
    sf::sleep(sf::milliseconds(600));

    // Fundido de salida
    for (int alpha = 200; alpha >= 0; alpha -= 10) {
        fadeRect.setFillColor(sf::Color(255, 240, 220, alpha));
        window.clear();
        window.draw(bgSprite);
        window.draw(fadeRect);
        if (alpha > 50) window.draw(text);
        window.display();
        sf::sleep(sf::milliseconds(12));
    }
}

void Game::nextLevel(sf::RenderWindow& window) {
    // Mostrar transición cremita ANTES de cargar el nuevo estado
    showLevelTransition(window, currentLevel + 1);

    if (currentLevel >= 3) {
        state = 2;
        gameOver = true;
        gameState = GameState::WinAll;
        return;
    }

    // Desbloquear para el jugador (si supera el nivel actual)
    // Se llama a nextLevel cuando ya cumplió objetivo del actual
    int unlockedNow = progress.getLevel(playerName);
    if (currentLevel + 1 > unlockedNow) {
        progress.setLevelUnlocked(playerName, currentLevel + 1);
    }

    ++currentLevel;
    counter = 20;
    movesSinceIce = 0;

    // CARGA DE NIVEL DESDE ARCHIVO
    LevelData data;
    std::string filename = "levels/nivel" + std::to_string(currentLevel) + ".txt";
    if (data.loadFromFile(filename)) {
        counter = data.movimientos;
        const_cast<int&>(TOTORO_GOAL) = data.totoroGoal;
        const_cast<int&>(ICE_GOAL) = data.iceGoal;
        const_cast<int&>(PONYO_GOAL) = data.ponyoGoal;

        if (!data.fondo.empty()) {
            bgTexture.loadFromFile(data.fondo);
            bgSprite.setTexture(bgTexture);
        }
    }

    totoroCleared = 0;
    iceCleared = 0;
    ponyoCleared = 0;
    board.setCounters(&totoroCleared, &iceCleared, &ponyoCleared);

    board.fillBoard();

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

<<<<<<< HEAD

void Game::checkLevelAdvance() {
=======
void Game::checkLevelAdvance(sf::RenderWindow& window) {
>>>>>>> develop
    bool done = false;
    if (currentLevel == 1) done = (totoroCleared >= TOTORO_GOAL);
    else if (currentLevel == 2) done = (iceCleared >= ICE_GOAL);
    else if (currentLevel == 3) done = (ponyoCleared >= PONYO_GOAL);

    if (done) {
        nextLevel(window);
    }
}

void Game::processCascadesOnce(sf::RenderWindow& window) {
    int cleared = board.findAndClearMatches();
    if (cleared > 0) {
        punctuation += cleared * 10;
        board.applyGravityAndRefill();
        updateHUD();
        checkLevelAdvance(window);
    }
}

<<<<<<< HEAD
=======
void Game::drawFinalScreen(sf::RenderWindow& window) {
    window.clear();
    window.draw(bgFinalSprite);

    // --- Título ---
    sf::Text title((gameState == GameState::WinAll) ? "¡Completaste todos los niveles!" : "Fin del Nivel", font, 42);
    title.setFillColor(sf::Color(0, 120, 0));
    centerText(title, window.getSize().x * 0.5f, 120);
    window.draw(title);

    // --- Puntaje del jugador ---
    sf::Text scoreLabel("Puntaje final: " + std::to_string(punctuation), font, 30);
    scoreLabel.setFillColor(sf::Color::White);
    centerText(scoreLabel, window.getSize().x * 0.5f, 190);
    window.draw(scoreLabel);

    // --- Ranking ---
    sf::Text rankTitle("Ranking (Top 5):", font, 26);
    rankTitle.setFillColor(sf::Color(0, 200, 0));
    centerText(rankTitle, window.getSize().x * 0.5f, 240);
    window.draw(rankTitle);

    auto top = ranking.getTopScores(5);
    float y = 280.f;
    int pos = 1;
    for (auto& s : top) {
        sf::Text line(std::to_string(pos) + ". " + s.name + " - " + std::to_string(s.score), font, 22);
        line.setFillColor(sf::Color::White);
        centerText(line, window.getSize().x * 0.5f, y);
        window.draw(line);
        y += 30.f;
        ++pos;
    }

    // --- Botones (centrados vertical) ---
    auto centerBtn = [&](sf::RectangleShape& r, sf::Text& t, float yPos) {
        r.setOrigin(r.getSize() * 0.5f);
        r.setPosition(window.getSize().x * 0.5f, yPos);
        auto lb = t.getLocalBounds();
        t.setOrigin(lb.left + lb.width * 0.5f, lb.top + lb.height * 0.5f);
        t.setPosition(r.getPosition());
        };

    centerBtn(restartButton, restartText, 600);
    centerBtn(menuButton, menuText, 720);
    centerBtn(exitButton, exitText, 840);

    window.draw(restartButton);
    window.draw(restartText);
    window.draw(menuButton);
    window.draw(menuText);
    window.draw(exitButton);
    window.draw(exitText);
}
>>>>>>> develop

void Game::run() {
    try {
        const int HUD_H = 50;
        RenderWindow window(VideoMode(880, 930), "Match Studio Ghibli");
        window.setFramerateLimit(60);

        board.centerInWindow(window.getSize().x, window.getSize().y, HUD_H);

        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
            throw std::string("Error: No se pudo cargar la fuente 'Arial' del sistema");

        // Fondos
        if (!bgTexture.loadFromFile("assets/FondoM.png"))
            throw std::string("Error: No se pudo cargar 'assets/FondoM.png'");
        if (!bgFinalTexture.loadFromFile("assets/Fondo2.png"))
            throw std::string("Error: No se pudo cargar 'assets/Fondo2.png'");
        bgSprite.setTexture(bgTexture);
        bgFinalSprite.setTexture(bgFinalTexture);
        coverSpriteToWindow(bgSprite, window.getSize());
        coverSpriteToWindow(bgFinalSprite, window.getSize());

        hudOk = true;
        scoreText.setFont(font);  scoreText.setCharacterSize(22); scoreText.setFillColor(Color::White);
        movesText.setFont(font);  movesText.setCharacterSize(22); movesText.setFillColor(Color::White);
        levelText.setFont(font);  levelText.setCharacterSize(22); levelText.setFillColor(Color::White);
        overText.setFont(font);   overText.setCharacterSize(40);  overText.setFillColor(Color(0, 100, 0));
        objectiveText.setFont(font); objectiveText.setCharacterSize(20); objectiveText.setFillColor(Color::White);

        // Botones estilo existente (verde)
        playButton.setSize({ 220.f, 90.f });   playButton.setFillColor(Color(0, 100, 0));
        restartButton.setSize({ 220.f, 90.f }); restartButton.setFillColor(Color(0, 100, 0));
        exitButton.setSize({ 220.f, 90.f });   exitButton.setFillColor(Color(0, 100, 0));
        menuButton.setSize({ 220.f, 90.f });   menuButton.setFillColor(Color(0, 100, 0));

        playText.setFont(font);    playText.setCharacterSize(32); playText.setFillColor(Color::White); playText.setString("Jugar");
        restartText.setFont(font); restartText.setCharacterSize(28); restartText.setFillColor(Color::White); restartText.setString("Reiniciar");
        exitText.setFont(font);    exitText.setCharacterSize(28); exitText.setFillColor(Color::White);    exitText.setString("Salir");
        menuText.setFont(font);    menuText.setCharacterSize(28); menuText.setFillColor(Color::White);    menuText.setString("Menú");

        // Posiciones (centradas cuando se dibuja la pantalla)
        // HUD / Estado inicial
        updateHUD();
<<<<<<< HEAD
        Clock clock;
        selR = selC = -1;

        std::string playerName;
        bool saved = false;
        sf::Uint32 lastChar = 0;

=======

        Clock clock;
        selR = selC = -1;

        // Entrada de nombre en menú principal
        playerName.clear();
        Uint32 lastChar = 0;

        // ===== Bucle principal =====
>>>>>>> develop
        while (window.isOpen()) {
            float dt = clock.restart().asSeconds();
            Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == Event::Closed) window.close();

<<<<<<< HEAD
                // ==========================
                // 🟢 MENÚ PRINCIPAL
                // ==========================
=======
                // ====== ESTADO: MENÚ PRINCIPAL ======
>>>>>>> develop
                if (state == 0) {
                    if (ev.type == Event::TextEntered) {
                        if (ev.text.unicode == '\b' && !playerName.empty()) {
                            playerName.pop_back();
                        }
                        else if (ev.text.unicode >= 32 && ev.text.unicode < 128 && playerName.size() < 12) {
                            // Evitar repeticiones por autorepeat
                            if (ev.text.unicode != lastChar) {
                                playerName += static_cast<char>(ev.text.unicode);
                                lastChar = ev.text.unicode;
                            }
                        }
                    }
                    if (ev.type == Event::KeyReleased) lastChar = 0;

                    if (ev.type == Event::KeyPressed && ev.key.code == Keyboard::Enter) {
                        // Leer progreso del jugador y habilitar niveles
                        int unlocked = progress.getLevel(playerName);
                        nivel1Unlocked = true;
                        nivel2Unlocked = (unlocked >= 2);
                        nivel3Unlocked = (unlocked >= 3);
                        state = 3; // Ir al menú de niveles
                    }

                    if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left) {
<<<<<<< HEAD
                        if (playButton.getGlobalBounds().contains((float)ev.mouseButton.x, (float)ev.mouseButton.y)) {
                            state = 1;
                            startGame(); // Carga nivel 1 automáticamente
=======
                        Vector2f m((float)ev.mouseButton.x, (float)ev.mouseButton.y);
                        if (playButton.getGlobalBounds().contains(m)) {
                            int unlocked = progress.getLevel(playerName);
                            nivel1Unlocked = true;
                            nivel2Unlocked = (unlocked >= 2);
                            nivel3Unlocked = (unlocked >= 3);
                            state = 3;
>>>>>>> develop
                        }
                    }
                }

<<<<<<< HEAD
                // ==========================
                // 🔵 ESTADO: JUGANDO
                // ==========================
=======
                // ====== ESTADO: MENÚ DE NIVELES ======
                else if (state == 3) {
                    if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left) {
                        // Capturar clic sobre botones de nivel
                        // (Las cajas se recalculan en el dibujo, ver más abajo)
                    }
                }

                // ====== ESTADO: JUGANDO ======
>>>>>>> develop
                else if (state == 1 && !gameOver && gameState == GameState::Playing) {
                    if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left) {
                        int r, c;
                        if (board.screenToCell(ev.mouseButton.x, ev.mouseButton.y, r, c)) {
                            if (selR == -1) {
                                Gem* g = board.getGem(r, c);
                                if (g) { selR = r; selC = c; }
                            }
                            else {
                                Gem* g1 = board.getGem(selR, selC);
                                Gem* g2 = board.getGem(r, c);

                                bool special1 = (g1 && g1->getIsSpecial() && !g1->getIsActivated());
                                bool special2 = (g2 && g2->getIsSpecial() && !g2->getIsActivated());

                                if (special1 || special2) {
                                    board.swapCells(selR, selC, r, c);

                                    Gem* g1_after = board.getGem(r, c);
                                    Gem* g2_after = board.getGem(selR, selC);

                                    if (special1 && g1_after) { g1_after->onMatch(board, r, c); punctuation += 50; }
                                    if (special2 && g2_after) { g2_after->onMatch(board, selR, selC); punctuation += 50; }

                                    waitingSpecial = true;

                                    ++movesSinceIce; --counter;
                                    if (movesSinceIce >= ICE_EVERY) { board.placeRandomIce(); movesSinceIce = 0; }

                                    updateHUD();
                                }
                                else if (board.isSwapValid(selR, selC, r, c)) {
                                    board.swapCells(selR, selC, r, c);
                                    ++movesSinceIce; --counter;
                                    if (movesSinceIce >= ICE_EVERY) { board.placeRandomIce(); movesSinceIce = 0; }

                                    int cleared = 0, safety = 0;
                                    do {
                                        cleared = board.findAndClearMatches();
                                        if (cleared > 0) { punctuation += cleared * 10; board.applyGravityAndRefill(); }
                                    } while (cleared > 0 && ++safety < 12);

                                    checkLevelAdvance(window);
                                    if (!gameOver && counter <= 0) { gameOver = true; state = 2; }
                                    updateHUD();
                                }
                                selR = selC = -1;
                            }
                        }
                    }
                }

<<<<<<< HEAD
                // ==========================
                // 🔴 PANTALLA FINAL (Ranking)
                // ==========================
                else if (state == 2) {
                    if (ev.type == sf::Event::TextEntered && !saved) {
                        if (ev.text.unicode == '\b' && !playerName.empty()) playerName.pop_back();
                        else if (ev.text.unicode >= 32 && ev.text.unicode < 128 && ev.text.unicode != lastChar && playerName.size() < 10) {
                            playerName += static_cast<char>(ev.text.unicode);
                            lastChar = ev.text.unicode;
                        }
                    }
                    if (ev.type == sf::Event::KeyReleased) lastChar = 0;
                    if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Enter && !saved) {
                        if (!playerName.empty()) {
                            ranking.addScore(playerName, punctuation);
                            saved = true;
                        }
                    }
=======
                // ====== ESTADO: FINAL / RANKING ======
                else if (state == 2) {
                    // Entrada de nombre para ranking (igual que tenías)
                    static bool saved = false;
                    static Uint32 lastC = 0;
                    if (gameState != GameState::WinAll) {
                        if (ev.type == Event::TextEntered && !saved) {
                            if (ev.text.unicode == '\b' && !playerName.empty()) playerName.pop_back();
                            else if (ev.text.unicode >= 32 && ev.text.unicode < 128 &&
                                ev.text.unicode != lastC && playerName.size() < 10) {
                                playerName += static_cast<char>(ev.text.unicode);
                                lastC = ev.text.unicode;
                            }
                        }
                        if (ev.type == Event::KeyReleased) lastC = 0;

                        if (ev.type == Event::KeyPressed && ev.key.code == Keyboard::Enter && !saved) {
                            if (!playerName.empty()) {
                                ranking.addScore(playerName, punctuation);
                                saved = true;
                            }
                        }
                    }

>>>>>>> develop
                    if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left) {
                        Vector2f m((float)ev.mouseButton.x, (float)ev.mouseButton.y);
                        if (restartButton.getGlobalBounds().contains(m)) {
                            // Reinicia al menú principal
                            state = 0;
                            punctuation = 0; counter = 20; gameOver = false;
                            currentLevel = 1; movesSinceIce = 0;
                            updateHUD();
                            playerName.clear();
                            saved = false;
                            lastChar = 0;
                        }
                        else if (menuButton.getGlobalBounds().contains(m)) {
                            // Volver al menú de niveles (manteniendo progreso)
                            int unlocked = progress.getLevel(playerName);
                            nivel1Unlocked = true;
                            nivel2Unlocked = (unlocked >= 2);
                            nivel3Unlocked = (unlocked >= 3);
                            state = 3;
                        }
                        else if (exitButton.getGlobalBounds().contains(m)) {
                            window.close();
                        }
                    }
                }
            } // fin pollEvent

<<<<<<< HEAD
            // --- Actualización de animaciones y dibujado ---
=======
            // ====== Actualización común de animaciones ======
>>>>>>> develop
            board.updateAnimations(dt);
            board.updateExplosions(dt);
            board.updateFlashes(dt);
            board.updateHighlight(dt);

<<<<<<< HEAD
=======
            // ====== Dibujo por estado ======
>>>>>>> develop
            window.clear();

            // ---- MENÚ PRINCIPAL ----
            if (state == 0) {
                window.draw(bgSprite);
<<<<<<< HEAD
=======

                // Campo visual "Nombre: ..."
                sf::Text nameLabel("Nombre: " + playerName + "_", font, 26);
                nameLabel.setFillColor(sf::Color(0, 100, 0));
                centerText(nameLabel, window.getSize().x * 0.5f, 340);
                window.draw(nameLabel);

                // Botón "Jugar"
                playButton.setOrigin(playButton.getSize() * 0.5f);
                playButton.setPosition(window.getSize().x * 0.5f, 430);
                playText.setOrigin(playText.getLocalBounds().left + playText.getLocalBounds().width * 0.5f,
                    playText.getLocalBounds().top + playText.getLocalBounds().height * 0.5f);
                playText.setPosition(playButton.getPosition());

>>>>>>> develop
                window.draw(playButton);
                window.draw(playText);
            }

            // ---- MENÚ DE NIVELES ----
            else if (state == 3) {
                window.draw(bgSprite);

                sf::Text title("Selecciona un nivel", font, 36);
                title.setFillColor(sf::Color::White);
                centerText(title, window.getSize().x * 0.5f, 120);
                window.draw(title);

                // Tres botones verticales
                sf::RectangleShape btn[3];
                sf::Text label[3];
                const char* names[3] = { "Nivel 1", "Nivel 2", "Nivel 3" };
                bool unlocked[3] = { nivel1Unlocked, nivel2Unlocked, nivel3Unlocked };

                for (int i = 0; i < 3; ++i) {
                    btn[i].setSize({ 220,90 });
                    btn[i].setOrigin(110, 45);
                    btn[i].setPosition(window.getSize().x * 0.5f, 260 + i * 130);
                    btn[i].setFillColor(unlocked[i] ? sf::Color(0, 120, 0) : sf::Color(60, 60, 60));

                    label[i].setFont(font);
                    label[i].setCharacterSize(26);
                    label[i].setString(names[i]);
                    label[i].setFillColor(sf::Color::White);
                    auto lb = label[i].getLocalBounds();
                    label[i].setOrigin(lb.left + lb.width * 0.5f, lb.top + lb.height * 0.5f);
                    label[i].setPosition(btn[i].getPosition());

                    window.draw(btn[i]);
                    window.draw(label[i]);
                }

                // Capturar clic aquí (tras dibujar, para usar bounds correctos)
                if (Mouse::isButtonPressed(Mouse::Left)) {
                    Vector2f m((float)Mouse::getPosition(window).x, (float)Mouse::getPosition(window).y);
                    for (int i = 0; i < 3; ++i) {
                        if (btn[i].getGlobalBounds().contains(m) && unlocked[i]) {
                            currentLevel = i + 1;
                            // Preparar tablero "nuevo" sin romper nada existente:
                            board.fillBoard();
                            startGame();
                            state = 1;
                            break;
                        }
                    }
                }
            }

            // ---- JUGANDO ----
            else if (state == 1) {
                // HUD
                RectangleShape hudBg({ (float)window.getSize().x, (float)HUD_H });
                hudBg.setFillColor(Color(0, 100, 0, 230));
                window.draw(hudBg);

                scoreText.setPosition(20, 5);
                objectiveText.setPosition(20, 28);
                movesText.setPosition((float)window.getSize().x - 240.f, 14.f);
                levelText.setPosition((float)window.getSize().x * 0.5f - 60.f, 14.f);

                window.draw(scoreText);
                window.draw(objectiveText);
                window.draw(movesText);
                window.draw(levelText);

                board.drawBoard(window);
                board.drawHighlight(window);

                if (gameState == GameState::Playing && selR != -1)
                    board.drawSelection(window, selR, selC);
<<<<<<< HEAD
            }
            else if (state == 2) {
                window.draw(bgFinalSprite);
                // Aquí va tu parte del ranking (idéntica a tu versión anterior)
            }

            // 🔸 Consumación del highlight de especiales
            if (waitingSpecial && !board.isHighlightActive()) {
                int cleared = board.clearHighlightedCells();
                if (cleared > 0) {
                    punctuation += cleared * 10;
                    board.applyGravityAndRefill();
                    int more = 0, safety = 0;
                    do {
                        more = board.findAndClearMatches();
                        if (more > 0) { punctuation += more * 10; board.applyGravityAndRefill(); }
                    } while (more > 0 && ++safety < 12);
                }
                checkLevelAdvance();
                if (!gameOver && counter <= 0) { gameOver = true; state = 2; }
                updateHUD();
                waitingSpecial = false;
=======

                // Consumación de highlight de especiales
                if (waitingSpecial && !board.isHighlightActive()) {
                    int cleared = board.clearHighlightedCells();
                    if (cleared > 0) {
                        punctuation += cleared * 10;
                        board.applyGravityAndRefill();

                        int more = 0, safety = 0;
                        do {
                            more = board.findAndClearMatches();
                            if (more > 0) {
                                punctuation += more * 10;
                                board.applyGravityAndRefill();
                            }
                        } while (more > 0 && ++safety < 12);
                    }

                    checkLevelAdvance(window);
                    if (!gameOver && counter <= 0) { gameOver = true; state = 2; }
                    updateHUD();
                    waitingSpecial = false;
                }
            }

            // ---- PANTALLA FINAL / RANKING ----
            else if (state == 2) {
                drawFinalScreen(window);
>>>>>>> develop
            }

            window.display();
        } // while open
    }
    catch (const std::exception& e) {
        std::cerr << "[❌ ERROR] " << e.what() << std::endl;
    }
}









map<string, Texture> TEXTURES;

Game::Game() : punctuation(0), counter(20) {}

void Game::updateHUD() {
    if (!hudOk) return;
    scoreText.setString("Puntos: " + to_string(punctuation));
    movesText.setString("Movimientos: " + to_string(counter));
}

void Game::run() {
    loadTexture("Totoro", "gemaTotoro.png");
    loadTexture("Ponyo", "gemaPonyo.png");
    loadTexture("Parti", "gemaParti.png");
    loadTexture("Gato", "gemaGato.png");
    loadTexture("Galleta", "gemaGalleta.png");

    int size = board.windowSize();
    sf::RenderWindow window(VideoMode(900, 900), "Match Studio Ghibli ");
    window.setFramerateLimit(60);

    if (font.loadFromFile("arial.ttf") ||
        font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        hudOk = true;

        scoreText.setFont(font);
        scoreText.setCharacterSize(20);
        scoreText.setFillColor(Color::White);
        scoreText.setPosition(8.f, 6.f);

        movesText.setFont(font);
        movesText.setCharacterSize(20);
        movesText.setFillColor(Color::White);
        movesText.setPosition((float)size - 220.f, 6.f);

        overText.setFont(font);
        overText.setCharacterSize(40);
        overText.setFillColor(Color(255, 230, 0));
        overText.setString("Listo reina");
        FloatRect b = overText.getLocalBounds();
        overText.setOrigin(b.width * 0.5f, b.height * 0.5f);
        overText.setPosition(size * 0.5f, size * 0.5f);

        playButton.setSize(Vector2f(200.f, 80.f));
        playButton.setFillColor(Color(144, 238, 144));
        playButton.setOrigin(100.f, 40.f);
        playButton.setPosition(450.f, 500.f);

        playText.setFont(font);
        playText.setString(" Jugar ");
        playText.setCharacterSize(30);
        playText.setFillColor(Color::White);
        FloatRect tb = playText.getLocalBounds();
        playText.setOrigin(tb.width * 0.5f, tb.height * 0.5f);
        playText.setPosition(playButton.getPosition());
    }
    if (!bgTexture.loadFromFile("FondoM.png")) {
        cout << "Error cargando fondo" << endl;
    }
    else {
        bgSprite.setTexture(bgTexture);
        FloatRect bgBounds = bgSprite.getLocalBounds();
        bgSprite.setScale(
            (float)window.getSize().x / bgBounds.width,
            (float)window.getSize().y / bgBounds.height
        );
    }


    int selR = -1, selC = -1;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();

            if (state == 0) {
                if (event.type == Event::MouseButtonPressed &&
                    event.mouseButton.button == Mouse::Left) {
                    float mx = (float)event.mouseButton.x;
                    float my = (float)event.mouseButton.y;

                    if (playButton.getGlobalBounds().contains(mx, my)) {
                        state = 1;
                        board.fillBoard();
                        updateHUD();
                    }
                }
            }
            else if (state == 1 && !gameOver) {
                if (event.type == Event::MouseButtonPressed &&
                    event.mouseButton.button == Mouse::Left) {
                    int mx = event.mouseButton.x;
                    int my = event.mouseButton.y;

                    int r, c;
                    if (board.screenToCell(mx, my, r, c)) {
                        if (selR == -1) {
                            selR = r; selC = c;
                        }
                        else {
                            board.swapCells(selR, selC, r, c);
                            int cleared = board.findAndClearMatches();
                            if (cleared > 0) {
                                int chain = 1;
                                punctuation += cleared * 10 * chain;

                                do {
                                    board.applyGravityAndRefill();
                                    cleared = board.findAndClearMatches();
                                    if (cleared > 0) {
                                        ++chain;
                                        punctuation += cleared * 10 * chain;
                                    }
                                } while (cleared > 0);
                                --counter;
                                if (counter <= 0) {
                                    counter = 0;
                                    gameOver = true;
                                    state = 2;
                                }
                                updateHUD();
                            }
                            else {
                                board.swapCells(selR, selC, r, c);
                            }
                            selR = selC = -1;
                        }
                    }
                }
            }
        }


        window.clear();
        window.draw(bgSprite);


        if (state == 0) {
            window.draw(playButton);
            window.draw(playText);
        }
        else if (state == 1) {
            board.drawBoard(window);

            if (!gameOver && selR != -1) {
                board.drawSelection(window, selR, selC);
            }

            if (hudOk) {
                RectangleShape bar(Vector2f((float)size, 30.f));
                bar.setFillColor(Color(0, 0, 0, 110));
                bar.setPosition(0.f, 0.f);
                window.draw(bar);
                window.draw(scoreText);
                window.draw(movesText);
            }
        }
        else if (state == 2) {
            board.drawBoard(window);

            RectangleShape shade(Vector2f((float)size, (float)size));
            shade.setFillColor(Color(0, 0, 0, 150));
            shade.setPosition(0.f, 0.f);
            window.draw(shade);
            window.draw(overText);
        }

        window.display();
    }
}

bool Game::loadTexture(const std::string& name, const string& path) {
    if (TEXTURES.find(name) != TEXTURES.end()) return true;

    Texture tex;
    if (!tex.loadFromFile(path)) {
        return false;
    }
    TEXTURES[name] = move(tex);
    return true;
}

