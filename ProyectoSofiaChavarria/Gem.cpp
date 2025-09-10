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
    float x = static_cast<float>(col * cellSize);
    float y = static_cast<float>(row * cellSize);

    cross1.setPosition(x, y);
    cross1.setSize(Vector2f(static_cast<float>(cellSize), 2.f));
    cross1.setRotation(45.f);

    cross2.setPosition(x + static_cast<float>(cellSize), y);
    cross2.setSize(Vector2f(static_cast<float>(cellSize), 2.f));
    cross2.setRotation(135.f);

    if (!loaded) return;

    FloatRect local = sprite.getLocalBounds();
    if (local.width > 0.f && local.height > 0.f) {
        float sx = static_cast<float>(cellSize) / local.width;
        float sy = static_cast<float>(cellSize) / local.height;
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

        // Escalar para que encaje en la celda
        auto bounds = sprite.getLocalBounds();
        float scale = (float)cellSize / bounds.width;
        sprite.setScale(scale, scale);

        // Posición
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
    this->imagePath = path;
    this->cellSize = cell;

    if (!loaded) {
        applyScaleAndPosition();
        cerr << "[WARN] No se pudo cargar: " << imagePath << "\n";
    }
}

void Gem::draw(sf::RenderWindow& window) {
  /*  sprite.setTexture(texture);
    applyScaleAndPosition();*/

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
