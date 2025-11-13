#include "Game.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include "Ranking.h"
#include "LevelData.h"
#include <fstream>

using namespace sf;
using namespace std;

Game::Game() {}
Game::~Game() {}

void Game::updateHUD() {
    if (!hudOk) return;
    scoreText.setString("Puntos: " + std::to_string(punctuation));
    movesText.setString("Movimientos: " + std::to_string(counter));
    levelText.setString("Nivel: " + std::to_string(currentLevel));

    std::string objText;
    if (currentLevel == 1)
        objText = "Objetivo: " + std::to_string(totoroCleared) + "/30 Totoro";
    else if (currentLevel == 2)
        objText = "Objetivo: " + std::to_string(iceCleared) + "/10 Hielo";
    else if (currentLevel == 3)
        objText = "Objetivo: " + std::to_string(ponyoCleared) + "/20 Ponyo";

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
    currentLevel = 1;
    movesSinceIce = 0;

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
    sf::Clock clock;
    float fade = 0.f;

    sf::RenderWindow temp(VideoMode(880, 930), "Match Studio Ghibli");
    temp.setFramerateLimit(60);
    sf::RectangleShape overlay(Vector2f(temp.getSize().x, temp.getSize().y));
    overlay.setFillColor(Color(0, 0, 0, 0));

    while (fade < 255) {
        fade += 400 * clock.restart().asSeconds();
        overlay.setFillColor(Color(0, 0, 0, (int)fade));
        temp.clear(Color::Black);
        temp.draw(overlay);
        temp.display();
    }

    if (currentLevel >= 3) {
        state = 2;
        gameOver = true;
        gameState = GameState::WinAll;
        return;
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


void Game::checkLevelAdvance() {
    bool done = false;
    if (currentLevel == 1) done = (totoroCleared >= TOTORO_GOAL);
    else if (currentLevel == 2) done = (iceCleared >= ICE_GOAL);
    else if (currentLevel == 3) done = (ponyoCleared >= PONYO_GOAL);

    if (done) nextLevel();
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


void Game::run() {
    try {
        const int HUD_H = 50;
        RenderWindow window(VideoMode(880, 930), "Match Studio Ghibli");
        window.setFramerateLimit(60);

        board.centerInWindow(window.getSize().x, window.getSize().y, HUD_H);

        if (!font.loadFromFile("arial.ttf") && !font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
            throw std::string("Error: No se pudo cargar la fuente 'arial.ttf'");

        if (!bgTexture.loadFromFile("assets/FondoM.png"))
            throw std::string("Error: No se pudo cargar 'assets/FondoM.png'");
        if (!bgFinalTexture.loadFromFile("assets/Fondo2.png"))
            throw std::string("Error: No se pudo cargar 'assets/Fondo2.png'");

        hudOk = true;

        scoreText.setFont(font);  scoreText.setCharacterSize(22); scoreText.setFillColor(Color::White);
        movesText.setFont(font);  movesText.setCharacterSize(22); movesText.setFillColor(Color::White);
        levelText.setFont(font);  levelText.setCharacterSize(22); levelText.setFillColor(Color::White);
        overText.setFont(font);   overText.setCharacterSize(40);  overText.setFillColor(Color(0, 100, 0));
        objectiveText.setFont(font); objectiveText.setCharacterSize(20); objectiveText.setFillColor(Color::White);

        playButton.setSize({ 220.f, 90.f }); playButton.setFillColor(Color(0, 100, 0));
        restartButton.setSize({ 220.f, 90.f }); restartButton.setFillColor(Color(0, 100, 0));
        exitButton.setSize({ 220.f, 90.f }); exitButton.setFillColor(Color(0, 100, 0));

        playText.setFont(font); playText.setCharacterSize(32); playText.setFillColor(Color::White); playText.setString("Jugar");
        restartText.setFont(font); restartText.setCharacterSize(28); restartText.setFillColor(Color::White); restartText.setString("Reiniciar");
        exitText.setFont(font); exitText.setCharacterSize(28); exitText.setFillColor(Color::White); exitText.setString("Salir");

        bgSprite.setTexture(bgTexture);
        coverSpriteToWindow(bgSprite, window.getSize());
        bgFinalSprite.setTexture(bgFinalTexture);
        coverSpriteToWindow(bgFinalSprite, window.getSize());

        auto centerText = [](Text& t, const Vector2f& p) {
            FloatRect b = t.getLocalBounds();
            t.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
            t.setPosition(p);
            };
        auto centerRect = [](RectangleShape& r, const Vector2f& p) {
            r.setOrigin(r.getSize() * 0.5f);
            r.setPosition(p);
            };

        centerRect(playButton, { (float)window.getSize().x * 0.5f, (float)window.getSize().y * 0.75f });
        centerText(playText, playButton.getPosition());
        centerRect(restartButton, { (float)window.getSize().x * 0.5f, (float)window.getSize().y * 0.58f });
        centerText(restartText, restartButton.getPosition());
        centerRect(exitButton, { (float)window.getSize().x * 0.5f, (float)window.getSize().y * 0.70f });
        centerText(exitText, exitButton.getPosition());

        updateHUD();
        Clock clock;
        selR = selC = -1;

        std::string playerName;
        bool saved = false;
        sf::Uint32 lastChar = 0;

        while (window.isOpen()) {
            float dt = clock.restart().asSeconds();
            Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == Event::Closed) window.close();

                // ==========================
                // 🟢 MENÚ PRINCIPAL
                // ==========================
                if (state == 0) {
                    if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left) {
                        if (playButton.getGlobalBounds().contains((float)ev.mouseButton.x, (float)ev.mouseButton.y)) {
                            state = 1;
                            startGame(); // Carga nivel 1 automáticamente
                        }
                    }
                }

                // ==========================
                // 🔵 ESTADO: JUGANDO
                // ==========================
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

                                    checkLevelAdvance();
                                    if (!gameOver && counter <= 0) { gameOver = true; state = 2; }
                                    updateHUD();
                                }
                                selR = selC = -1;
                            }
                        }
                    }
                }

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
                    if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left) {
                        Vector2f m{ (float)ev.mouseButton.x, (float)ev.mouseButton.y };
                        if (restartButton.getGlobalBounds().contains(m)) {
                            state = 0;
                            punctuation = 0; counter = 20; gameOver = false;
                            currentLevel = 1; movesSinceIce = 0;
                            updateHUD();
                            playerName.clear();
                            saved = false;
                            lastChar = 0;
                        }
                        else if (exitButton.getGlobalBounds().contains(m)) {
                            window.close();
                        }
                    }
                }
            }

            // --- Actualización de animaciones y dibujado ---
            board.updateAnimations(dt);
            board.updateExplosions(dt);
            board.updateFlashes(dt);
            board.updateHighlight(dt);

            window.clear();

            if (state == 0) {
                window.draw(bgSprite);
                window.draw(playButton);
                window.draw(playText);
            }
            else if (state == 1) {
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
            }

            window.display();
        }

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

