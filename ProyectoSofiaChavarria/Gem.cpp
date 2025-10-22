#include "Gem.h"
#include "Board.h"
#include <iostream>
#include <cmath>

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
    if (isSpecial && !isActivated) {
        float t = std::sin(clock() * 0.002f) * 50 + 205;
        sprite.setColor(sf::Color(255, (sf::Uint8)t, (sf::Uint8)t, (sf::Uint8)opacity));
    }
    else {
        sprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)opacity));
    }
    if (!empty) window.draw(sprite);
}

// 🔹 Animación general
void Gem::update(float dt) {
    // Fade-out si está vacía
    if (empty) {
        opacity -= 400.f * dt;
        if (opacity < 0.f) opacity = 0.f;
        sprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)opacity));
        return;
    }

    // Caída suave
    if (falling) {
        yOffset += fallSpeed * dt;
        fallSpeed += 900.f * dt; // Aceleración (gravedad)
        sprite.move(0.f, fallSpeed * dt);

        if (yOffset >= Board::CELL) {
            falling = false;
            yOffset = 0.f;
            sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - (Board::CELL));
        }
    }
}
