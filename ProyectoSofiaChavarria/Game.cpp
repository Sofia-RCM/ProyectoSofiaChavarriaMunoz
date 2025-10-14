#include "Game.h"
#include <iostream>
using namespace std;
using namespace sf;

map<string, Texture> TEXTURES;

Game::Game() : punctuation(0), counter(20) {}

void Game::updateHUD() {
    if (!hudOk) return;
    scoreText.setString("Puntos: " + to_string(punctuation));
    movesText.setString("Movimientos: " + to_string(counter));
}

void Game::run() {
    loadTexture("Totoro", "assets/gemaTotoro.png");
    loadTexture("Ponyo", "assets/gemaPonyo.png");
    loadTexture("Parti", "assets/gemaParti.png");
    loadTexture("Gato", "assets/gemaGato.png");
    loadTexture("Galleta", "assets/gemaGalleta.png");

    int size = board.windowSize();
    int hudHeight = 50;

    RenderWindow window(VideoMode(880, 930), "Match Studio Ghibli ");
    window.setFramerateLimit(60);

    if (font.loadFromFile("arial.ttf") ||
        font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        hudOk = true;

        scoreText.setFont(font);
        scoreText.setCharacterSize(20);
        scoreText.setFillColor(Color::White);

        movesText.setFont(font);
        movesText.setCharacterSize(20);
        movesText.setFillColor(Color::White);

        overText.setFont(font);
        overText.setCharacterSize(40);
        overText.setFillColor(Color(0, 100, 0));
        overText.setString("¡Juego terminado!");
        FloatRect b = overText.getLocalBounds();
        overText.setOrigin(b.width * 0.5f, b.height * 0.5f);
        overText.setPosition(440, 300);

        playButton.setSize(Vector2f(200, 80));
        playButton.setFillColor(Color(0, 100, 0));
        playButton.setOrigin(100, 40);
        playButton.setPosition(440, 500);

        playText.setFont(font);
        playText.setString("Jugar");
        playText.setCharacterSize(30);
        playText.setFillColor(Color::White);
        FloatRect tb = playText.getLocalBounds();
        playText.setOrigin(tb.width * 0.5f, tb.height * 0.5f);
        playText.setPosition(playButton.getPosition());

        restartButton.setSize(Vector2f(200, 80));
        restartButton.setFillColor(Color(0, 100, 0));
        restartButton.setOrigin(100, 40);
        restartButton.setPosition(440, 500);

        restartText.setFont(font);
        restartText.setString("Reiniciar");
        restartText.setCharacterSize(28);
        restartText.setFillColor(Color::White);
        FloatRect rt = restartText.getLocalBounds();
        restartText.setOrigin(rt.width * 0.5f, rt.height * 0.5f);
        restartText.setPosition(restartButton.getPosition());

        exitButton.setSize(Vector2f(200, 80));
        exitButton.setFillColor(Color(0, 100, 0));
        exitButton.setOrigin(100, 40);
        exitButton.setPosition(440, 600);

        exitText.setFont(font);
        exitText.setString("Salir");
        exitText.setCharacterSize(28);
        exitText.setFillColor(Color::White);
        FloatRect et = exitText.getLocalBounds();
        exitText.setOrigin(et.width * 0.5f, et.height * 0.5f);
        exitText.setPosition(exitButton.getPosition());
    }

    bgTexture.loadFromFile("assets/FondoM.png");
    bgSprite.setTexture(bgTexture);
    FloatRect bgBounds = bgSprite.getLocalBounds();
    bgSprite.setScale(
        (float)window.getSize().x / bgBounds.width,
        (float)window.getSize().y / bgBounds.height
    );

    bgFinalTexture.loadFromFile("assets/Fondo2.png");
    bgFinalSprite.setTexture(bgFinalTexture);
    FloatRect bgBounds2 = bgFinalSprite.getLocalBounds();
    bgFinalSprite.setScale(
        (float)window.getSize().x / bgBounds2.width,
        (float)window.getSize().y / bgBounds2.height
    );

    board.setOffset(0, hudHeight);

    int selR = -1, selC = -1;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();

            if (state == 0) {
                if (event.type == Event::MouseButtonPressed &&
                    event.mouseButton.button == Mouse::Left) {
                    float mx = event.mouseButton.x;
                    float my = event.mouseButton.y;

                    if (playButton.getGlobalBounds().contains(mx, my)) {
                        state = 1;
                        punctuation = 0;
                        counter = 20;
                        gameOver = false; 8;
                        board.fillBoard();

                        int cleared = board.findAndClearMatches();
                        while (cleared > 0) {
                            int chain = 1;
                            punctuation += cleared * 10 * chain;
                            board.applyGravityAndRefill();
                            cleared = board.findAndClearMatches();
                        }

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
            else if (state == 2) {
                if (event.type == Event::MouseButtonPressed &&
                    event.mouseButton.button == Mouse::Left) {
                    float mx = event.mouseButton.x;
                    float my = event.mouseButton.y;

                    if (restartButton.getGlobalBounds().contains(mx, my)) {
                        punctuation = 0;
                        counter = 20;
                        gameOver = false;
                        updateHUD();
                        state = 0;
                    }
                    else if (exitButton.getGlobalBounds().contains(mx, my)) {
                        window.close();
                    }
                }
            }
        }

        window.clear();

        if (state == 0) {
            window.draw(bgSprite);
            window.draw(playButton);
            window.draw(playText);
        }
        else if (state == 1) {
            board.drawBoard(window);

            if (!gameOver && selR != -1) {
                board.drawSelection(window, selR, selC);
            }

            if (hudOk) {
                RectangleShape hudBg(Vector2f(window.getSize().x, hudHeight));
                hudBg.setFillColor(Color(0, 100, 0, 220));
                hudBg.setPosition(0, 0);
                window.draw(hudBg);

                scoreText.setPosition(20, 15);
                movesText.setPosition(window.getSize().x - 200, 15);

                window.draw(scoreText);
                window.draw(movesText);
            }
        }
        else if (state == 2) {
            string finalMsg = "Puntos: " + to_string(punctuation);
            overText.setString(finalMsg);

            FloatRect b = overText.getLocalBounds();
            overText.setOrigin(b.width * 0.5f, b.height * 0.5f);
            overText.setPosition(440, 300);

            window.draw(bgFinalSprite);
            window.draw(overText);
            window.draw(restartButton);
            window.draw(restartText);
            window.draw(exitButton);
            window.draw(exitText);
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
