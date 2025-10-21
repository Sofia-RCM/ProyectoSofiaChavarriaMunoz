#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Board;

class Gem {
protected:
    std::string tipoGem;
    sf::Sprite sprite;
    sf::Texture texture;
    bool empty = false;
    bool isSpecial = false;
    bool isActivated = false;

public:
    Gem();
    virtual ~Gem();

    virtual void draw(sf::RenderWindow& window);
    virtual void setTipoGem(const std::string& tipo);
    virtual std::string getTipoGem() const { return tipoGem; }

    virtual void setGrid(int fila, int columna, int cellSize, float offsetX, float offsetY);
    virtual bool isEmpty() const { return empty; }

    virtual void onMatch(Board& board, int row, int col) {}

    bool getIsSpecial() const { return isSpecial; }
    void setIsSpecial(bool special) { isSpecial = special; }
    bool getIsActivated() const { return isActivated; }
    void setIsActivated(bool activated) { isActivated = activated; }

    sf::Sprite& getSprite() { return sprite; }
};