#pragma once
#include "Gem.h"
#include "Board.h"

class GalletaGem : public Gem {
public:
    GalletaGem();
    virtual void onMatch(Board& board, int row, int col) override;
};
