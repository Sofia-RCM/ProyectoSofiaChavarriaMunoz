#pragma once
#include "Gem.h"

class PonyoGem : public Gem {
public:
    PonyoGem();
    void setTipoGem(const std::string& tipo) override;
    void onMatch(Board& board, int row, int col) override;
};