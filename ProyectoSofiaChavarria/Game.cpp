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
    sf::RenderWindow window(VideoMode(900, 1000), "Match Studio Ghibli ");
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

        // Botón de inicio
        playButton.setSize(Vector2f(200.f, 80.f));
        playButton.setFillColor(Color(70, 130, 180));
        playButton.setOrigin(100.f, 40.f);
        playButton.setPosition(450.f, 500.f);

        playText.setFont(font);
        playText.setString("JUGAR");
        playText.setCharacterSize(30);
        playText.setFillColor(Color::White);
        FloatRect tb = playText.getLocalBounds();
        playText.setOrigin(tb.width * 0.5f, tb.height * 0.5f);
        playText.setPosition(playButton.getPosition());
    }

    int selR = -1, selC = -1;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();

            if (state == 0) { // MENU
                if (event.type == Event::MouseButtonPressed &&
                    event.mouseButton.button == Mouse::Left) {
                    float mx = (float)event.mouseButton.x;
                    float my = (float)event.mouseButton.y;

                    if (playButton.getGlobalBounds().contains(mx, my)) {
                        state = 1; // PLAYING
                        board.fillBoard();
                        updateHUD();
                    }
                }
            }
            else if (state == 1 && !gameOver) { // PLAYING
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
                                    state = 2; // GAME_OVER
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

        // Renderizado
        window.clear(Color(10, 10, 10));

        if (state == 0) { // MENU
            window.draw(playButton);
            window.draw(playText);
        }
        else if (state == 1) { // PLAYING
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
        else if (state == 2) { // GAME_OVER
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
