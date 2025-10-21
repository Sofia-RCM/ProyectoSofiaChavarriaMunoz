#pragma once
#include "Gem.h"

class PartiGem : public Gem {
public:
    PartiGem();
    void setTipoGem(const std::string& tipo) override;
    void onMatch(Board& board, int row, int col) override;
};