#include "HUD.h"

HUD::HUD(Font& f) : font(f), score(0), moves(0) {
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(Color::White);

    movesText.setFont(font);
    movesText.setCharacterSize(20);
    movesText.setFillColor(Color::White);
}

void HUD::setScore(int s) { score = s; }
void HUD::setMoves(int m) { moves = m; }

void HUD::draw(RenderWindow& window, int width, int hudHeight) {
    RectangleShape bg(Vector2f(width, hudHeight));
    bg.setFillColor(Color(0, 100, 0, 220));
    bg.setPosition(0, 0);
    window.draw(bg);

    scoreText.setString("Puntos: " + to_string(score));
    movesText.setString("Movimientos: " + to_string(moves));

    scoreText.setPosition(20, 15);
    movesText.setPosition(width - 200, 15);

    window.draw(scoreText);
    window.draw(movesText);
}
