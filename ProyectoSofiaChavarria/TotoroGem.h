#pragma once
#include "Gem.h"

class TotoroGem : public Gem {
public:
    TotoroGem();
    void setTipoGem(const std::string& tipo) override;
    void onMatch(Board& board, int row, int col) override;
};