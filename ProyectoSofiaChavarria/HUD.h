#pragma once

#include <SFML/Graphics.hpp>
#include <string>
using namespace std;
using namespace sf;

class HUD {
private:
    Text scoreText;
    Text movesText;
    Font font;
    int score;
    int moves;

public:
    HUD(Font& f);
    void setScore(int s);
    void setMoves(int m);
    void draw(RenderWindow& window, int windowWidth, int hudHeight);
};

