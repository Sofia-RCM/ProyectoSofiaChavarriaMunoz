#include "Gem.h"
#include <iostream>

Gem::Gem() {}
Gem::~Gem() {}

void Gem::setTipoGem(const std::string& tipo) {
    tipoGem = tipo;

    // USAMOS PREFIJO "gema"
    std::string ruta = "assets/gema" + tipo + ".png";

    if (!texture.loadFromFile(ruta)) {
        std::cerr << "[❌] Error al cargar textura: " << ruta << std::endl;
        empty = true;
        return;
    }

    sprite.setTexture(texture);
    // Escala que encaje bien en CELL=96
    sprite.setScale(0.1f, 0.1f);

    // Si el nombre contiene "Especial" es especial
    isSpecial = (tipo.find("Especial") != std::string::npos);
    isActivated = false;
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
