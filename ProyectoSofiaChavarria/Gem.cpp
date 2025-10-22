#include "Gem.h"
#include <iostream>

Gem::Gem() {}
Gem::~Gem() {}

void Gem::setTipoGem(const std::string& tipo) {
    tipoGem = tipo;
    std::string ruta = "assets/gema" + tipo + ".png";

    if (!texture.loadFromFile(ruta)) {
        throw "Error: No se pudo cargar la textura: " + ruta;
    }

    sprite.setTexture(texture);
    sprite.setScale(0.1f, 0.1f);

    isSpecial = (tipo.find("Especial") != std::string::npos);
    isActivated = false;
    empty = false;
}

void Gem::setGrid(int fila, int columna, int cellSize, float offsetX, float offsetY) {
    float cx = offsetX + columna * cellSize + cellSize * 0.5f;
    float cy = offsetY + fila * cellSize + cellSize * 0.5f;

    sf::Vector2u ts = texture.getSize();
    float w = ts.x * sprite.getScale().x;
    float h = ts.y * sprite.getScale().y;

    sprite.setPosition(cx - w * 0.5f, cy - h * 0.5f);
}

void Gem::draw(sf::RenderWindow& window) {
    if (!empty) window.draw(sprite);
}