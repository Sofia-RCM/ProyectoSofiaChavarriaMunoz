#pragma once
#include "Gem.h"

class NormalGem : public Gem {
public:
    NormalGem(const string& name) {
        type = name;
        sprite.setTexture(ResourceManager::getTexture(name));
        loaded = true;
    }
    void draw(RenderWindow& window) override { window.draw(sprite); }
    string getType() const override { return type; }
};
