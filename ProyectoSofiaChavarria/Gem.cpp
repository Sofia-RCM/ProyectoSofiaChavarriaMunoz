#include "Gem.h"
#include "Board.h"
#include <iostream>

Gem::Gem() {}

Gem::~Gem() {}

void Gem::setTipoGem(const std::string& tipo) {
    tipoGem = tipo;
    std::string ruta = "assets/gema" + tipo + ".png";

    if (!texture.loadFromFile(ruta)) {
        empty = true;
        return;
    }

    sprite.setTexture(texture);
    sprite.setScale(0.1f, 0.1f); // Prueba con 0.3
}

void Gem::setGrid(int fila, int columna, int cellSize, float offsetX, float offsetY) {
    float cellCenterX = offsetX + columna * cellSize + cellSize * 0.5f;
    float cellCenterY = offsetY + fila * cellSize + cellSize * 0.5f;

    sf::FloatRect bounds = sprite.getGlobalBounds();
    float width = bounds.width;
    float height = bounds.height;

    float x = cellCenterX - width * 0.5f;
    float y = cellCenterY - height * 0.5f;

    sprite.setPosition(x, y);
}

void Gem::draw(sf::RenderWindow& window) {
    if (!empty)
        window.draw(sprite);
}