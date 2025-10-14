#pragma once
#include "Gem.h"

class NormalGem : public Gem {
public:
    NormalGem();
    void onMatch() override;
    string getType() const override { return "Normal"; }
};
