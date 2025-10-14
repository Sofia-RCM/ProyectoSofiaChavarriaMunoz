#pragma once
#include "Gem.h"

class BombGem : public Gem {
public:
    BombGem();
    void onMatch() override;
    string getType() const override { return "Bomb"; }
};
