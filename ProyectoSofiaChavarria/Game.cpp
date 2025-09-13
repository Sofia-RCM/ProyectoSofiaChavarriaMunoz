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
        overText.setFillColor(Color(0, 100, 0));
        overText.setString("Juego terminado");
        FloatRect b = overText.getLocalBounds();
        overText.setOrigin(b.width * 0.5f, b.height * 0.5f);
        overText.setPosition(450.f, 300.f);

        playButton.setSize(Vector2f(200.f, 80.f));
        playButton.setFillColor(Color(144, 238, 144));
        playButton.setOrigin(100.f, 40.f);
        playButton.setPosition(450.f, 500.f);

        playText.setFont(font);
        playText.setString("Jugar");
        playText.setCharacterSize(30);
        playText.setFillColor(Color::White);
        FloatRect tb = playText.getLocalBounds();
        playText.setOrigin(tb.width * 0.5f, tb.height * 0.5f);
        playText.setPosition(playButton.getPosition());

        restartButton.setSize(Vector2f(200.f, 80.f));
        restartButton.setFillColor(Color(144, 238, 144));
        restartButton.setOrigin(100.f, 40.f);
        restartButton.setPosition(450.f, 500.f);

        restartText.setFont(font);
        restartText.setString("Reiniciar");
        restartText.setCharacterSize(28);
        restartText.setFillColor(Color::White);
        FloatRect rt = restartText.getLocalBounds();
        restartText.setOrigin(rt.width * 0.5f, rt.height * 0.5f);
        restartText.setPosition(restartButton.getPosition());
    }

    bgTexture.loadFromFile("FondoM.png");
    bgSprite.setTexture(bgTexture);
    FloatRect bgBounds = bgSprite.getLocalBounds();
    bgSprite.setScale(
        (float)window.getSize().x / bgBounds.width,
        (float)window.getSize().y / bgBounds.height
    );

    bgFinalTexture.loadFromFile("Fondo2.png");
    bgFinalSprite.setTexture(bgFinalTexture);
    FloatRect bgBounds2 = bgFinalSprite.getLocalBounds();
    bgFinalSprite.setScale(
        (float)window.getSize().x / bgBounds2.width,
        (float)window.getSize().y / bgBounds2.height
    );

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
                        punctuation = 0;
                        counter = 20;
                        gameOver = false;
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
            else if (state == 2) {
                if (event.type == Event::MouseButtonPressed &&
                    event.mouseButton.button == Mouse::Left) {
                    float mx = (float)event.mouseButton.x;
                    float my = (float)event.mouseButton.y;

                    if (restartButton.getGlobalBounds().contains(mx, my)) {
                        punctuation = 0;
                        counter = 20;
                        gameOver = false;
                        updateHUD();
                        state = 0;
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
                RectangleShape bar(Vector2f((float)size, 30.f));
                bar.setFillColor(Color(0, 0, 0, 110));
                bar.setPosition(0.f, 0.f);
                window.draw(bar);
                window.draw(scoreText);
                window.draw(movesText);
            }
        }
        else if (state == 2) {
            window.draw(bgFinalSprite);
            window.draw(overText);
            window.draw(restartButton);
            window.draw(restartText);
        }

        window.display();
    }
}

bool Game::loadTexture(const string& name, const string& path) {
    if (TEXTURES.find(name) != TEXTURES.end()) return true;

    Texture tex;
    if (!tex.loadFromFile(path)) {
        return false;
    }
    TEXTURES[name] = move(tex);
    return true;
}
