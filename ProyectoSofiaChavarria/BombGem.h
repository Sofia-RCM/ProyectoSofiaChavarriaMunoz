#pragma once
#include "Gem.h"

class BombGem : public Gem {
public:
    BombGem(const string& name = "Bomb") {
        type = name;
        sprite.setTexture(ResourceManager::getTexture("Galleta"));
        loaded = true;
    }
    void onMatch(Board& board) override;
    void draw(RenderWindow& window) override { window.draw(sprite); }
    string getType() const override { return type; }
    bool isSpecial() const override { return true; }
};
