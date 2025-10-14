#include "Gem.h"
#include "Game.h"
using namespace std;
using namespace sf;

void Gem::applyScaleAndPosition() {
    float x = col * cellSize;
    float y = row * cellSize;
    if (!loaded) return;
    FloatRect local = sprite.getLocalBounds();
    if (local.width > 0 && local.height > 0) {
        float sx = cellSize / local.width;
        float sy = cellSize / local.height;
        float base = (sx < sy ? sx : sy);
        float s = base * 0.85f;
        sprite.setScale(s, s);
        sprite.setOrigin(local.width / 2, local.height / 2);
        sprite.setPosition(x + cellSize / 2, y + cellSize / 2);
    }
}

void Gem::setTipoGem(string& tipo) {
    tipoGem = tipo;
    loaded = true;
    if (TEXTURES.find(tipo) != TEXTURES.end()) {
        sprite.setTexture(TEXTURES[tipo]);
        applyScaleAndPosition();
    }
}

string Gem::getTipoGem() { return tipoGem; }

void Gem::setGrid(int r, int c, int cell) {
    row = r;
    col = c;
    cellSize = cell;
    applyScaleAndPosition();
}

void Gem::setPosition(float cx, float cy) {
    if (loaded) sprite.setPosition(cx, cy);
}

void Gem::draw(RenderWindow& window) {
    if (loaded) window.draw(sprite);
}

bool Gem::isEmpty() { return tipoGem.empty(); }
bool Gem::isLoaded() { return loaded; }
