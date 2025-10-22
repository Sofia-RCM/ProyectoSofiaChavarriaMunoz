#include "Game.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace sf;

Game::Game() {
    transitionOverlay.setSize({ 880.f, 930.f });
    transitionOverlay.setFillColor(Color(255, 220, 150, 0));

    transitionText.setCharacterSize(60);
    transitionText.setFillColor(Color(255, 255, 255, 0));
    transitionText.setString("¡Nivel Completado!");
}
Game::~Game() {}

void Game::updateHUD() {
    if (!hudOk) return;
    scoreText.setString("Puntos: " + std::to_string(punctuation));
    movesText.setString("Movimientos: " + std::to_string(counter));
    levelText.setString("Nivel: " + std::to_string(currentLevel));

    std::string obj;
    if (currentLevel == 1) obj = "Objetivo: " + std::to_string(totoroCleared) + "/30 Totoro";
    else if (currentLevel == 2) obj = "Objetivo: " + std::to_string(iceCleared) + "/10 Hielo";
    else if (currentLevel == 3) obj = "Objetivo: " + std::to_string(ponyoCleared) + "/20 Ponyo";
    objectiveText.setString(obj);
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
    currentLevel = 1;
    movesSinceIce = 0;

    totoroCleared = 0;
    iceCleared = 0;
    ponyoCleared = 0;
    board.setCounters(&totoroCleared, &iceCleared, &ponyoCleared);

    gameOver = false;
    gameState = GameState::Playing;

    int cleared = board.findAndClearMatches();
    int safety = 0;
    while (cleared > 0 && safety++ < 12) {
        punctuation += cleared * 10;
        board.applyGravityAndRefill();
        cleared = board.findAndClearMatches();
    }
    updateHUD();
}

void Game::nextLevel() {
    if (currentLevel >= 3) {
        state = 2;
        gameOver = true;
        gameState = GameState::WinAll;
        return;
    }
    ++currentLevel;
    counter = 20;
    movesSinceIce = 0;

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

void Game::checkLevelAdvance() {
    bool done = false;
    if (currentLevel == 1) done = (totoroCleared >= TOTORO_GOAL);
    else if (currentLevel == 2) done = (iceCleared >= ICE_GOAL);
    else if (currentLevel == 3) done = (ponyoCleared >= PONYO_GOAL);

    if (done && !levelTransitionActive) {
        startLevelTransition();
        pendingNextLevel = true;
    }
}

void Game::processCascadesOnce() {
    int cleared = board.findAndClearMatches();
    if (cleared > 0) {
        punctuation += cleared * 10;
        board.applyGravityAndRefill();
        updateHUD();
        checkLevelAdvance();
    }
}

void Game::startLevelTransition() {
    // Activar transición
    levelTransitionActive = true;
    transitionTimer = 0.f;

    // Configuración del brillo dorado de fondo
    transitionOverlay.setSize({ 880.f, 930.f });
    transitionOverlay.setFillColor(sf::Color(255, 220, 150, 0));

    // Configuración del texto animado
    transitionText.setFont(font);
    transitionText.setString("¡Nivel Completado!");
    transitionText.setCharacterSize(60);
    transitionText.setFillColor(sf::Color(255, 255, 255, 0));

    // Centrar texto en pantalla
    sf::FloatRect textRect = transitionText.getLocalBounds();
    transitionText.setOrigin(textRect.width / 2.f, textRect.height / 2.f);
    transitionText.setPosition(440.f, 465.f);
}

void Game::updateLevelTransition(float dt) {
    if (!levelTransitionActive) return;

    transitionTimer += dt;
    int overlayAlpha = 0;
    int textAlpha = 0;

    // Etapa 1: aparición (0 - 0.5s)
    if (transitionTimer < 0.5f) {
        float t = transitionTimer / 0.5f;
        overlayAlpha = static_cast<int>(t * 200);
        textAlpha = static_cast<int>(t * 255);
    }
    // Etapa 2: pausa visible (0.5 - 1.0s)
    else if (transitionTimer < 1.0f) {
        overlayAlpha = 200;
        textAlpha = 255;
    }
    // Etapa 3: desvanecimiento y desplazamiento del texto (1.0 - 2.0s)
    else if (transitionTimer < 2.0f) {
        float t = (2.0f - transitionTimer);
        overlayAlpha = static_cast<int>(200 * t);
        textAlpha = static_cast<int>(255 * t);
        transitionText.move(0.f, -40.f * dt); // sube suavemente
    }
    // Etapa final: pasar de nivel
    else {
        levelTransitionActive = false;
        transitionText.setPosition(440.f, 465.f);
        if (pendingNextLevel) {
            pendingNextLevel = false;
            nextLevel();
        }
        return;
    }

    // Aplicar colores actualizados
    transitionOverlay.setFillColor(sf::Color(255, 220, 150, overlayAlpha));
    transitionText.setFillColor(sf::Color(255, 255, 255, textAlpha));
}


void Game::drawLevelTransition(sf::RenderWindow& window) {
    if (levelTransitionActive) {
        window.draw(transitionOverlay);
        window.draw(transitionText);
    }
}
void Game::run() {
    try {
        const int HUD_H = 50;
        RenderWindow window(VideoMode(880, 930), "Match Studio Ghibli");
        window.setFramerateLimit(60);

        board.centerInWindow(window.getSize().x, window.getSize().y, HUD_H);

        if (!font.loadFromFile("arial.ttf") && !font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            throw std::string("Error: No se pudo cargar la fuente 'arial.ttf'");
        }
        if (!bgTexture.loadFromFile("assets/FondoM.png")) {
            throw std::string("Error: No se pudo cargar 'assets/FondoM.png'");
        }
        if (!bgFinalTexture.loadFromFile("assets/Fondo2.png")) {
            throw std::string("Error: No se pudo cargar 'assets/Fondo2.png'");
        }

        hudOk = true;
        scoreText.setFont(font);  scoreText.setCharacterSize(22); scoreText.setFillColor(Color::White);
        movesText.setFont(font);  movesText.setCharacterSize(22); movesText.setFillColor(Color::White);
        levelText.setFont(font);  levelText.setCharacterSize(22); levelText.setFillColor(Color::White);
        overText.setFont(font);   overText.setCharacterSize(40);  overText.setFillColor(Color(0, 100, 0));
        objectiveText.setFont(font); objectiveText.setCharacterSize(20); objectiveText.setFillColor(Color::White);

        playButton.setSize({ 220.f, 90.f });    playButton.setFillColor(Color(0, 100, 0));
        restartButton.setSize({ 220.f, 90.f }); restartButton.setFillColor(Color(0, 100, 0));
        exitButton.setSize({ 220.f, 90.f });    exitButton.setFillColor(Color(0, 100, 0));

        playText.setFont(font);    playText.setCharacterSize(32); playText.setFillColor(Color::White); playText.setString("Jugar");
        restartText.setFont(font); restartText.setCharacterSize(28); restartText.setFillColor(Color::White); restartText.setString("Reiniciar");
        exitText.setFont(font);    exitText.setCharacterSize(28);  exitText.setFillColor(Color::White);  exitText.setString("Salir");

        bgSprite.setTexture(bgTexture);     coverSpriteToWindow(bgSprite, window.getSize());
        bgFinalSprite.setTexture(bgFinalTexture); coverSpriteToWindow(bgFinalSprite, window.getSize());

        auto centerTextFn = [](Text& t, const Vector2f& p) {
            FloatRect b = t.getLocalBounds();
            t.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
            t.setPosition(p);
            };
        auto centerRectFn = [](RectangleShape& r, const Vector2f& p) {
            r.setOrigin(r.getSize() * 0.5f);
            r.setPosition(p);
            };

        centerRectFn(playButton, { (float)window.getSize().x * 0.5f, (float)window.getSize().y * 0.75f });
        centerTextFn(playText, playButton.getPosition());
        centerRectFn(restartButton, { (float)window.getSize().x * 0.5f, (float)window.getSize().y * 0.58f });
        centerTextFn(restartText, restartButton.getPosition());
        centerRectFn(exitButton, { (float)window.getSize().x * 0.5f, (float)window.getSize().y * 0.70f });
        centerTextFn(exitText, exitButton.getPosition());

        updateHUD();
        Clock clock;
        selR = selC = -1;

        while (window.isOpen()) {
            float dt = clock.restart().asSeconds();
            updateLevelTransition(dt);
            Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == Event::Closed) window.close();

                if (state == 0) {
                    if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left) {
                        if (playButton.getGlobalBounds().contains((float)ev.mouseButton.x, (float)ev.mouseButton.y)) {
                            state = 1;
                            startGame();
                        }
                    }
                }
                else if (state == 1 && !gameOver && gameState == GameState::Playing) {
                    if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left) {
                        int r, c;
                        if (board.screenToCell(ev.mouseButton.x, ev.mouseButton.y, r, c)) {
                            if (selR == -1) {
                                if (board.getGem(r, c)) { selR = r; selC = c; }
                            }
                            else {
                                auto* g1 = board.getGem(selR, selC);
                                auto* g2 = board.getGem(r, c);

                                bool special1 = g1 && g1->getIsSpecial() && !g1->getIsActivated();
                                bool special2 = g2 && g2->getIsSpecial() && !g2->getIsActivated();

                                if (special1 || special2) {
                                    board.swapCells(selR, selC, r, c);

                                    auto* g1a = board.getGem(r, c);
                                    auto* g2a = board.getGem(selR, selC);

                                    if (special1 && g1a) { g1a->onMatch(board, r, c); punctuation += 50; }
                                    if (special2 && g2a) { g2a->onMatch(board, selR, selC); punctuation += 50; }

                                    board.applyGravityAndRefill();
                                    ++movesSinceIce; --counter;
                                    if (movesSinceIce >= ICE_EVERY) { board.placeRandomIce(); movesSinceIce = 0; }

                                    int cleared = 0, safety = 0;
                                    do {
                                        cleared = board.findAndClearMatches();
                                        if (cleared > 0) { punctuation += cleared * 10; board.applyGravityAndRefill(); }
                                    } while (cleared > 0 && ++safety < 12);

                                    checkLevelAdvance();
                                    if (!gameOver && counter <= 0) { counter = 0; gameOver = true; state = 2; }
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

                                    checkLevelAdvance();
                                    if (!gameOver && counter <= 0) { counter = 0; gameOver = true; state = 2; }
                                    updateHUD();
                                }
                                selR = selC = -1;
                            }
                        }
                    }
                }
                else if (state == 2) {
                    if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left) {
                        Vector2f m{ (float)ev.mouseButton.x, (float)ev.mouseButton.y };
                        if (restartButton.getGlobalBounds().contains(m)) {
                            state = 0;
                            punctuation = 0; counter = 20; gameOver = false;
                            currentLevel = 1; movesSinceIce = 0;
                            updateHUD();
                        }
                        else if (exitButton.getGlobalBounds().contains(m)) {
                            window.close();
                        }
                    }
                }
            }

            window.clear();

            if (state == 0) {
                window.draw(bgSprite);
                window.draw(playButton); window.draw(playText);
            }
            else if (state == 1) {
                RectangleShape hudBg({ (float)window.getSize().x, (float)HUD_H });
                hudBg.setFillColor(Color(0, 100, 0, 230));
                window.draw(hudBg);

                scoreText.setPosition(20, 14);
                objectiveText.setPosition(20, 40);
                movesText.setPosition((float)window.getSize().x - 240.f, 14.f);
                levelText.setPosition((float)window.getSize().x * 0.5f - 60.f, 14.f);

                window.draw(scoreText);
                window.draw(objectiveText);
                window.draw(movesText);
                window.draw(levelText);

                board.drawBoard(window);
                if (gameState == GameState::Playing && selR != -1)
                    board.drawSelection(window, selR, selC);

                drawLevelTransition(window);
            }
            else if (state == 2) {
                window.draw(bgFinalSprite);
                std::string msg = (gameState == GameState::WinAll)
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

            window.display();
        }

    }
    catch (const char* msg) {
#ifdef _WIN32
        MessageBoxA(nullptr, msg, "Error Crítico", MB_ICONERROR);
#endif
    }
    catch (const std::string& msg) {
#ifdef _WIN32
        MessageBoxA(nullptr, msg.c_str(), "Error Crítico", MB_ICONERROR);
#endif
    }
    catch (...) {
#ifdef _WIN32
        MessageBoxA(nullptr, "Error desconocido. El juego no puede continuar.", "Error", MB_ICONERROR);
#endif
    }
}
