#pragma once
#include "Gem.h"

class IceGem : public Gem {
private:
    int durability;
public:
    IceGem() {
        type = "Ice";
        sprite.setTexture(ResourceManager::getTexture("Ponyo"));
        loaded = true;
        durability = 2;
    }
    void onMatch(Board& board) override {
        durability--;
    }
    void draw(RenderWindow& window) override { window.draw(sprite); }
    string getType() const override { return type; }
    bool isSpecial() const override { return true; }
};
