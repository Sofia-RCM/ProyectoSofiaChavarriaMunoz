#pragma once
#include "Gem.h"

class IceGem : public Gem {
private:
    int hitsRemaining;
public:
    IceGem();
    void onMatch() override;
    string getType() const override { return "Ice"; }
    int getHits() const { return hitsRemaining; }
};
