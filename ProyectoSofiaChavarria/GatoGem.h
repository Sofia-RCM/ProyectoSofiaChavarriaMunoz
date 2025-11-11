#pragma once
#include "Gem.h"
#include "Board.h"

class GatoGem : public Gem {
public:
    GatoGem();
    virtual void onMatch(Board& board, int row, int col) override;
};
