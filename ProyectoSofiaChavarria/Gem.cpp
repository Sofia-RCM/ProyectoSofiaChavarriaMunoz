#include "Gem.h"
#include "Board.h"
#include <iostream>

Gem::Gem() {}

Gem::~Gem() {}

void Gem::setTipoGem(const std::string& tipo) {
    tipoGem = tipo;
    std::string ruta = "assets/gema" + tipo + ".png";

    if (!texture.loadFromFile(ruta)) {
        std::cerr << "[❌] Error: No se pudo cargar: " << ruta << std::endl;
        empty = true;
        return;
    }

    sprite.setTexture(texture);
    sprite.setScale(0.1f, 0.1f);

    isSpecial = (tipo.find("Especial") != std::string::npos);
    isActivated = false;
}

void Gem::setGrid(int fila, int columna, int cellSize, float offsetX, float offsetY) {
    float cellCenterX = offsetX + columna * cellSize + cellSize * 0.5f;
    float cellCenterY = offsetY + fila * cellSize + cellSize * 0.5f;

    sf::Vector2u textureSize = texture.getSize();
    float scaledWidth = textureSize.x * sprite.getScale().x;
    float scaledHeight = textureSize.y * sprite.getScale().y;

    float x = cellCenterX - scaledWidth * 0.5f;
    float y = cellCenterY - scaledHeight * 0.5f;
    sprite.setPosition(x, y);
}

void Gem::draw(sf::RenderWindow& window) {
    if (!empty)
        window.draw(sprite);
}