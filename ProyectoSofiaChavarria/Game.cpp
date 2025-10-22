#include "Game.h"
#include <iostream>
using namespace sf;

Game::Game() {}
Game::~Game() {}

void Game::updateHUD() {
    if (!hudOk) return;
    scoreText.setString("Puntos: " + std::to_string(punctuation));
    movesText.setString("Movimientos: " + std::to_string(counter));
    levelText.setString("Nivel: " + std::to_string(currentLevel) +
        "  Meta: " + std::to_string(levelGoals[currentLevel - 1]));
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
    counter = 20;           // restaurar movimientos
    movesSinceIce = 0;

    board.fillBoard();      // tablero nuevo
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
    if (punctuation >= levelGoals[currentLevel - 1]) {
        nextLevel(); // pasa automáticamente
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

void Game::run() {
    const int HUD_H = 50;
    RenderWindow window(VideoMode(880, 930), "Match Studio Ghibli");
    window.setFramerateLimit(60);

    board.centerInWindow(window.getSize().x, window.getSize().y, HUD_H);

    if (font.loadFromFile("arial.ttf") || font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        hudOk = true;
        scoreText.setFont(font);  scoreText.setCharacterSize(22); scoreText.setFillColor(Color::White);
        movesText.setFont(font);  movesText.setCharacterSize(22); movesText.setFillColor(Color::White);
        levelText.setFont(font);  levelText.setCharacterSize(22); levelText.setFillColor(Color::White);
        overText.setFont(font);   overText.setCharacterSize(40);  overText.setFillColor(Color(0, 100, 0));

        playButton.setSize({ 220.f, 90.f });    playButton.setFillColor(Color(0, 100, 0));
        restartButton.setSize({ 220.f, 90.f }); restartButton.setFillColor(Color(0, 100, 0));
        exitButton.setSize({ 220.f, 90.f });    exitButton.setFillColor(Color(0, 100, 0));

        playText.setFont(font);    playText.setCharacterSize(32);    playText.setFillColor(Color::White);  playText.setString("Jugar");
        restartText.setFont(font); restartText.setCharacterSize(28); restartText.setFillColor(Color::White); restartText.setString("Reiniciar");
        exitText.setFont(font);    exitText.setCharacterSize(28);    exitText.setFillColor(Color::White);   exitText.setString("Salir");
    }

    bgTexture.loadFromFile("assets/FondoM.png");
    bgSprite.setTexture(bgTexture);
    coverSpriteToWindow(bgSprite, window.getSize());

    bgFinalTexture.loadFromFile("assets/Fondo2.png");
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

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

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

                                if (special1) { g1->onMatch(board, r, c); punctuation += 50; }
                                if (special2) { g2->onMatch(board, selR, selC); punctuation += 50; }

                                board.applyGravityAndRefill(); // limpia espacios si activaron clears
                                ++movesSinceIce;
                                --counter;
                                if (movesSinceIce >= ICE_EVERY) { board.placeRandomIce(); movesSinceIce = 0; }

                                // cascadas automáticas
                                int cleared = 0, safety = 0;
                                do {
                                    cleared = board.findAndClearMatches();
                                    if (cleared > 0) {
                                        punctuation += cleared * 10;
                                        board.applyGravityAndRefill();
                                    }
                                } while (cleared > 0 && ++safety < 12);

                                checkLevelAdvance();
                                if (!gameOver && counter <= 0) { gameOver = true; state = 2; }
                                updateHUD();
                            }
                            else if (board.isSwapValid(selR, selC, r, c)) {
                                board.swapCells(selR, selC, r, c);

                                ++movesSinceIce;
                                --counter;
                                if (movesSinceIce >= ICE_EVERY) { board.placeRandomIce(); movesSinceIce = 0; }

                                int cleared = 0, safety = 0;
                                do {
                                    cleared = board.findAndClearMatches();
                                    if (cleared > 0) {
                                        punctuation += cleared * 10;
                                        board.applyGravityAndRefill();
                                    }
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
            else if (state == 2) {
                if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left) {
                    sf::Vector2f m{ (float)ev.mouseButton.x, (float)ev.mouseButton.y };
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
            sf::RectangleShape hudBg({ (float)window.getSize().x, (float)HUD_H });
            hudBg.setFillColor(sf::Color(0, 100, 0, 230));
            window.draw(hudBg);

            scoreText.setPosition(20, 14);
            movesText.setPosition((float)window.getSize().x - 240.f, 14.f);
            levelText.setPosition((float)window.getSize().x * 0.5f - 60.f, 14.f);

            window.draw(scoreText);
            window.draw(movesText);
            window.draw(levelText);

            board.drawBoard(window);
            if (gameState == GameState::Playing && selR != -1)
                board.drawSelection(window, selR, selC);
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
