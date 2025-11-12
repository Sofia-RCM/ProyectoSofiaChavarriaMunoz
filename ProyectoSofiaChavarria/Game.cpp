#include "Game.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include <fstream>
#include <sstream>

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
void Game::nextLevel(sf::RenderWindow& window) {
    // --- Mostrar pantalla de carga tipo "transición" ---
    sf::Font font;
    // Intentar cargar fuente del proyecto, y si no existe, usar la del sistema
    if (!font.loadFromFile("assets/fuente.ttf") &&
        !font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        throw std::string("Error: No se pudo cargar ninguna fuente válida (ni 'assets/fuente.ttf' ni Arial)");
    }

    // Texto dinámico con número de nivel
    std::string msg = "Cargando nivel " + std::to_string(currentLevel + 1) + "...";
    sf::Text loading;
    loading.setFont(font);
    loading.setString(msg);
    loading.setCharacterSize(46);
    loading.setFillColor(sf::Color(30, 90, 30)); // Verde Ghibli
    loading.setStyle(sf::Text::Bold);

    // Fondo color cremita
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(240, 230, 210)); // crema suave

    // Centrar correctamente
    sf::FloatRect textBounds = loading.getLocalBounds();
    loading.setOrigin(textBounds.left + textBounds.width / 2.f,
        textBounds.top + textBounds.height / 2.f);
    loading.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

    // Mostrar pantalla de transición
    window.clear(sf::Color(240, 230, 210));
    window.draw(background);
    window.draw(loading);
    window.display();

    // Mantener visible antes de cargar el siguiente nivel
    sf::sleep(sf::seconds(2.f));

    // --- Lógica normal de cambio de nivel ---
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

    // ⚡ Este bloque tarda — por eso mostramos antes el mensaje
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



void Game::checkLevelAdvance(sf::RenderWindow& window) {
    bool done = false;
    if (currentLevel == 1) done = (totoroCleared >= TOTORO_GOAL);
    else if (currentLevel == 2) done = (iceCleared >= ICE_GOAL);
    else if (currentLevel == 3) done = (ponyoCleared >= PONYO_GOAL);

    if (done) nextLevel(window); 
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

void Game::run() {
    try {
        const int HUD_H = 50;
        sf::RenderWindow window(sf::VideoMode(880, 930), "Match Studio Ghibli");
        window.setFramerateLimit(60);

        board.centerInWindow(window.getSize().x, window.getSize().y, HUD_H);

        if (!font.loadFromFile("arial.ttf") && !font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
            throw std::string("Error: No se pudo cargar la fuente 'arial.ttf'");

        if (!bgTexture.loadFromFile("assets/FondoM.png"))
            throw std::string("Error: No se pudo cargar 'assets/FondoM.png'");
        if (!bgFinalTexture.loadFromFile("assets/Fondo2.png"))
            throw std::string("Error: No se pudo cargar 'assets/Fondo2.png'");

        hudOk = true;

        // Textos y botones base (mismos estilos)
        scoreText.setFont(font);  scoreText.setCharacterSize(22); scoreText.setFillColor(sf::Color::White);
        movesText.setFont(font);  movesText.setCharacterSize(22); movesText.setFillColor(sf::Color::White);
        levelText.setFont(font);  levelText.setCharacterSize(22); levelText.setFillColor(sf::Color::White);
        overText.setFont(font);   overText.setCharacterSize(40);  overText.setFillColor(sf::Color(0, 100, 0));
        objectiveText.setFont(font); objectiveText.setCharacterSize(20); objectiveText.setFillColor(sf::Color::White);

        playButton.setSize({ 220.f, 90.f }); playButton.setFillColor(sf::Color(0, 100, 0));
        restartButton.setSize({ 220.f, 90.f }); restartButton.setFillColor(sf::Color(0, 100, 0));
        exitButton.setSize({ 220.f, 90.f }); exitButton.setFillColor(sf::Color(0, 100, 0));

        playText.setFont(font); playText.setCharacterSize(32); playText.setFillColor(sf::Color::White); playText.setString("Jugar");
        restartText.setFont(font); restartText.setCharacterSize(28); restartText.setFillColor(sf::Color::White); restartText.setString("Reiniciar");
        exitText.setFont(font); exitText.setCharacterSize(28); exitText.setFillColor(sf::Color::White); exitText.setString("Salir");

        bgSprite.setTexture(bgTexture);
        coverSpriteToWindow(bgSprite, window.getSize());
        bgFinalSprite.setTexture(bgFinalTexture);
        coverSpriteToWindow(bgFinalSprite, window.getSize());

        auto centerText = [](sf::Text& t, const sf::Vector2f& p) {
            sf::FloatRect b = t.getLocalBounds();
            t.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
            t.setPosition(p);
            };
        auto centerRect = [](sf::RectangleShape& r, const sf::Vector2f& p) {
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
        sf::Clock clock;
        selR = selC = -1;

        // Variables para pantalla final (ranking)
        std::string playerName;
        bool saved = false;
        sf::Uint32 lastChar = 0;

        while (window.isOpen()) {
            float dt = clock.restart().asSeconds();

            // --------------------- EVENTOS ---------------------
            sf::Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == sf::Event::Closed) window.close();

                // Menú principal
                if (state == 0) {
                    if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
                        if (playButton.getGlobalBounds().contains((float)ev.mouseButton.x, (float)ev.mouseButton.y)) {
                            state = 3; // ir a menú de niveles
                        }
                    }
                }
                // Jugando
                else if (state == 1 && !gameOver && gameState == GameState::Playing) {
                    if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
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
                // Pantalla final (entrada de nombre + botones)
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
                        if (!playerName.empty()) { ranking.addScore(playerName, punctuation); saved = true; }
                    }
                    if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2f m{ (float)ev.mouseButton.x, (float)ev.mouseButton.y };
                        if (restartButton.getGlobalBounds().contains(m)) {
                            state = 0;
                            punctuation = 0; counter = 20; gameOver = false;
                            currentLevel = 1; movesSinceIce = 0;
                            updateHUD();
                            playerName.clear(); saved = false; lastChar = 0;
                        }
                        else if (exitButton.getGlobalBounds().contains(m)) {
                            window.close();
                        }
                    }
                }
            } // fin eventos

            // --------------------- UPDATE ---------------------
            board.updateAnimations(dt);
            board.updateExplosions(dt);
            board.updateFlashes(dt);
            board.updateHighlight(dt);

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
                checkLevelAdvance(window);

                if (!gameOver && counter <= 0) { gameOver = true; state = 2; }
                updateHUD();
                waitingSpecial = false;
            }

            // --------------------- DRAW ---------------------
            window.clear();

            // MENÚ PRINCIPAL
            if (state == 0) {
                window.draw(bgSprite);
                window.draw(playButton);
                window.draw(playText);
            }
            // MENÚ DE NIVELES (dibujado SIEMPRE aquí → sin parpadeo)
            else if (state == 3) {
                window.draw(bgSprite);

                // Título centrado
                sf::Text title("Selecciona un nivel", font, 36);
                title.setFillColor(sf::Color::White);
                title.setPosition(window.getSize().x / 2.f - title.getLocalBounds().width / 2.f, 80);
                window.draw(title);

                // Botones centrados
                float centerX = window.getSize().x / 2.f - 110.f;
                float btnW = 220.f, btnH = 90.f, spacing = 40.f;

                sf::RectangleShape btn1({ btnW, btnH });
                sf::RectangleShape btn2({ btnW, btnH });
                sf::RectangleShape btn3({ btnW, btnH });

                btn1.setPosition(centerX, 220);
                btn2.setPosition(centerX, 220 + btnH + spacing);
                btn3.setPosition(centerX, 220 + (btnH + spacing) * 2);

                auto setColor = [](bool unlocked, bool completed) {
                    if (completed) return sf::Color(0, 180, 0);
                    if (unlocked) return sf::Color(220, 180, 0);
                    return sf::Color(80, 80, 80);
                    };
                btn1.setFillColor(setColor(nivel1Unlocked, nivel1Completado));
                btn2.setFillColor(setColor(nivel2Unlocked, nivel2Completado));
                btn3.setFillColor(setColor(nivel3Unlocked, nivel3Completado));

                window.draw(btn1); window.draw(btn2); window.draw(btn3);

                sf::Text t1("Nivel 1", font, 26);
                sf::Text t2("Nivel 2", font, 26);
                sf::Text t3("Nivel 3", font, 26);
                t1.setFillColor(sf::Color::White);
                t2.setFillColor(sf::Color::White);
                t3.setFillColor(sf::Color::White);
                t1.setPosition(centerX + 60, 250);
                t2.setPosition(centerX + 60, 250 + btnH + spacing);
                t3.setPosition(centerX + 60, 250 + (btnH + spacing) * 2);
                window.draw(t1); window.draw(t2); window.draw(t3);

                // Botón "Volver"
                sf::RectangleShape backBtn({ 160.f, 60.f });
                backBtn.setFillColor(sf::Color(0, 100, 0));
                backBtn.setPosition(window.getSize().x - 180.f, window.getSize().y - 90.f);
                window.draw(backBtn);
                sf::Text backTxt("Volver", font, 22);
                backTxt.setFillColor(sf::Color::White);
                backTxt.setPosition(backBtn.getPosition().x + 40.f, backBtn.getPosition().y + 16.f);
                window.draw(backTxt);

                // Detectar clics (simple y robusto)
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    sf::Vector2f m((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y);
                    if (btn1.getGlobalBounds().contains(m) && nivel1Unlocked) { currentLevel = 1; startGame(); state = 1; }
                    else if (btn2.getGlobalBounds().contains(m) && nivel2Unlocked) { currentLevel = 2; startGame(); state = 1; }
                    else if (btn3.getGlobalBounds().contains(m) && nivel3Unlocked) { currentLevel = 3; startGame(); state = 1; }
                    else if (backBtn.getGlobalBounds().contains(m)) { state = 0; }
                }
            }
            // JUGANDO
            else if (state == 1) {
                sf::RectangleShape hudBg({ (float)window.getSize().x, (float)HUD_H });
                hudBg.setFillColor(sf::Color(0, 100, 0, 230));
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
            // PANTALLA FINAL
            else if (state == 2) {
                window.draw(bgFinalSprite);

                sf::Text title("¡Nivel completado!", font, 38);
                title.setFillColor(sf::Color(0, 100, 0));
                title.setPosition(window.getSize().x / 2.f - title.getLocalBounds().width / 2.f, 150);
                window.draw(title);

                sf::Text scoreLabel("Puntuación final: " + std::to_string(punctuation), font, 26);
                scoreLabel.setFillColor(sf::Color::Black);
                scoreLabel.setPosition(window.getSize().x / 2.f - scoreLabel.getLocalBounds().width / 2.f, 230);
                window.draw(scoreLabel);

                sf::Text enterName("Ingrese su nombre:", font, 22);
                enterName.setFillColor(sf::Color::Black);
                enterName.setPosition(window.getSize().x / 2.f - enterName.getLocalBounds().width / 2.f, 290);
                window.draw(enterName);

                sf::Text nameText(playerName, font, 28);
                nameText.setFillColor(sf::Color(0, 80, 0));
                nameText.setPosition(window.getSize().x / 2.f - nameText.getLocalBounds().width / 2.f, 320);
                window.draw(nameText);

                if (saved) {
                    sf::Text savedText("Puntaje guardado correctamente", font, 20);
                    savedText.setFillColor(sf::Color(0, 150, 0));
                    savedText.setPosition(window.getSize().x / 2.f - savedText.getLocalBounds().width / 2.f, 360);
                    window.draw(savedText);
                }

                float centerX = window.getSize().x / 2.f;
                float btnWidth = 220.f, btnHeight = 90.f, spacing = 20.f;

                restartButton.setSize({ btnWidth, btnHeight });
                restartButton.setFillColor(sf::Color(0, 100, 0));
                restartButton.setOrigin(btnWidth / 2.f, btnHeight / 2.f);
                restartButton.setPosition(centerX, 460);
                window.draw(restartButton);

                restartText.setFont(font);
                restartText.setCharacterSize(28);
                restartText.setFillColor(sf::Color::White);
                restartText.setString("Reiniciar");
                {
                    sf::FloatRect tb = restartText.getLocalBounds();
                    restartText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
                    restartText.setPosition(restartButton.getPosition());
                }
                window.draw(restartText);
                exitButton.setSize({ btnWidth, btnHeight });
                exitButton.setFillColor(sf::Color(0, 100, 0));
                exitButton.setOrigin(btnWidth / 2.f, btnHeight / 2.f);
                exitButton.setPosition(centerX, 460 + btnHeight + spacing);
                window.draw(exitButton);

                exitText.setFont(font);
                exitText.setCharacterSize(28);
                exitText.setFillColor(sf::Color::White);
                exitText.setString("Salir");
                // Centramos el texto dentro del botón
                {
                    sf::FloatRect tb = exitText.getLocalBounds();
                    exitText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
                    exitText.setPosition(exitButton.getPosition());
                }
                window.draw(exitText);
                sf::Text rankTitle("Ranking Top 10:", font, 26);
                rankTitle.setFillColor(sf::Color(0, 80, 0));
                rankTitle.setPosition(centerX - rankTitle.getLocalBounds().width / 2.f, 630);
                window.draw(rankTitle);

                int y = 660;
                for (auto& s : ranking.getScores()) {
                    sf::Text line(s.name + " - " + std::to_string(s.score), font, 22);
                    line.setFillColor(sf::Color::Black);
                    line.setPosition(centerX - line.getLocalBounds().width / 2.f, (float)y);
                    window.draw(line);
                    y += 26;
                }
            }

            window.display();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[❌ ERROR] " << e.what() << std::endl;
    }
}



// 🟢 Leer progreso desde progress.txt
void Game::loadProgress() {
    std::ifstream file("progress.txt");
    if (!file.is_open()) {
        std::cerr << "⚠️ No se encontró progress.txt, se usará progreso por defecto.\n";
        nivel1Unlocked = true;
        nivel2Unlocked = nivel3Unlocked = false;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.rfind("nivel1=", 0) == 0) nivel1Unlocked = (line.back() == '1');
        else if (line.rfind("nivel2=", 0) == 0) nivel2Unlocked = (line.back() == '1');
        else if (line.rfind("nivel3=", 0) == 0) nivel3Unlocked = (line.back() == '1');
    }
    file.close();
}

// 🔵 Guardar progreso en progress.txt
void Game::saveProgress() {
    std::ofstream file("progress.txt", std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "❌ No se pudo abrir progress.txt para guardar.\n";
        return;
    }
    file << "nivel1=" << (nivel1Unlocked ? 1 : 0) << "\n";
    file << "nivel2=" << (nivel2Unlocked ? 1 : 0) << "\n";
    file << "nivel3=" << (nivel3Unlocked ? 1 : 0) << "\n";
    file.close();
}

void Game::showLevelTransition(sf::RenderWindow& window, int nextLevel) {
    sf::RectangleShape fadeRect(sf::Vector2f(window.getSize()));
    fadeRect.setFillColor(sf::Color(255, 240, 220, 0)); // Beige suave

    sf::Font font;
    font.loadFromFile("assets/arial.ttf");

    sf::Text text("Nivel " + std::to_string(nextLevel), font, 60);
    text.setFillColor(sf::Color(34, 102, 34)); // Verde Ghibli
    sf::FloatRect tb = text.getLocalBounds();
    text.setOrigin(tb.width / 2, tb.height / 2);
    text.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

    // Fundido de entrada
    for (int alpha = 0; alpha <= 255; alpha += 5) {
        fadeRect.setFillColor(sf::Color(255, 240, 220, alpha));
        window.clear();
        window.draw(fadeRect);
        if (alpha > 120) window.draw(text);
        window.display();
        sf::sleep(sf::milliseconds(20));
    }

    sf::sleep(sf::seconds(1.5f));

    // Fundido de salida
    for (int alpha = 255; alpha >= 0; alpha -= 5) {
        fadeRect.setFillColor(sf::Color(255, 240, 220, alpha));
        window.clear();
        window.draw(fadeRect);
        if (alpha > 50) window.draw(text);
        window.display();
        sf::sleep(sf::milliseconds(15));
    }
}
