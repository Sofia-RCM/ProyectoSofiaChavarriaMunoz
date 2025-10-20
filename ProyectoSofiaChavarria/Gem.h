#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Board; // Declaración adelantada

class Gem {
protected:
    std::string tipoGem;
    sf::Sprite sprite;
    sf::Texture texture;
    bool empty = false;

public:
    Gem();
    virtual ~Gem();

    virtual void draw(sf::RenderWindow& window);
    virtual void setTipoGem(const std::string& tipo);
    virtual std::string getTipoGem() const { return tipoGem; }

    virtual void setGrid(int fila, int columna, int cellSize, float offsetX, float offsetY);
    virtual bool isEmpty() const { return empty; }

    // Polimorfismo: comportamiento al hacer match
    virtual void onMatch(Board& board, int row, int col) {}

    sf::Sprite& getSprite() { return sprite; }
};