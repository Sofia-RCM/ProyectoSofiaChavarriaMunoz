#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Board;

class Gem {
protected:
    std::string tipoGem;
    sf::Sprite   sprite;
    sf::Texture  texture;
    bool empty = false;
    bool isSpecial = false;
    bool isActivated = false;

public:
    Gem();
    virtual ~Gem();

    // Render
    virtual void draw(sf::RenderWindow& window);

    // Tipo / textura
    virtual void setTipoGem(const std::string& tipo);
    virtual std::string getTipoGem() const { return tipoGem; }

    // Ubicación en la grilla
    virtual void setGrid(int fila, int columna, int cellSize, float offsetX, float offsetY);

    // Estado
    virtual bool isEmpty() const { return empty; }

    // Poder (sobrecargar en derivadas)
    virtual void onMatch(Board& board, int row, int col) {}

    // Accesores de flags
    bool getIsSpecial() const { return isSpecial; }
    void setIsSpecial(bool v) { isSpecial = v; }

    bool getIsActivated() const { return isActivated; }
    void setIsActivated(bool v) { isActivated = v; }

    sf::Sprite& getSprite() { return sprite; }
};
