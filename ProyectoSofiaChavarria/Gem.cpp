#include "Gem.h"
#include <iostream>
#include "Game.h"
using namespace std;
using namespace sf;

Gem::Gem() {
    cross1.setFillColor(Color::Magenta);
    cross2.setFillColor(Color::Magenta);
}

void Gem::applyScaleAndPosition() {
    float x = col * cellSize;
    float y = row * cellSize;

    cross1.setPosition(x, y);
    cross1.setSize(Vector2f(cellSize, 2));
    cross1.setRotation(45);

    cross2.setPosition(x + cellSize, y);
    cross2.setSize(Vector2f(cellSize, 2));
    cross2.setRotation(135);

    if (!loaded) return;

    FloatRect local = sprite.getLocalBounds();
    if (local.width > 0 && local.height > 0) {
        float sx = cellSize / local.width;
        float sy = cellSize / local.height;
        float base = (sx < sy ? sx : sy);
        float s = base * 0.85f;
        sprite.setScale(s, s);

        local = sprite.getLocalBounds();
        sprite.setOrigin(local.width * 0.5f, local.height * 0.5f);
        sprite.setPosition(x + cellSize * 0.5f, y + cellSize * 0.5f);
    }
}

void Gem::setTipoGem(string& tipo) {
    tipoGem = tipo;
    loaded = true;

    if (TEXTURES.find(tipo) != TEXTURES.end()) {
        sprite.setTexture(TEXTURES[tipo]);
        auto bounds = sprite.getLocalBounds();
        float scale = cellSize / bounds.width;
        sprite.setScale(scale, scale);
        sprite.setPosition(col * cellSize, row * cellSize);
    }
}

string Gem::getTipoGem() { return tipoGem; }
string Gem::getImage() { return imagePath; }
bool   Gem::isLoaded() { return loaded; }

void Gem::setGrid(int r, int c, int cell) {
    row = r;
    col = c;
    cellSize = cell;
    applyScaleAndPosition();
}

void Gem::setImage(string path, int cell) {
    imagePath = path;
    cellSize = cell;
    if (!loaded) {
        applyScaleAndPosition();
    }
}

void Gem::setPosition(float cx, float cy) {
    if (loaded) {
        sprite.setPosition(cx, cy);
    }
}

void Gem::draw(RenderWindow& window) {
    if (loaded) {
        window.draw(sprite);
    }
    else {
        window.draw(cross1);
        window.draw(cross2);
    }
}

bool Gem::isEmpty() {
    return tipoGem.empty();
}
