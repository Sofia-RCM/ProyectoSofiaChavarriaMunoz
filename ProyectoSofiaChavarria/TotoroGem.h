#pragma once
#include "Gem.h"
#include "Board.h"

class TotoroGem : public Gem {
public:
    TotoroGem();
    virtual void onMatch(Board& board, int row, int col) override;
};
