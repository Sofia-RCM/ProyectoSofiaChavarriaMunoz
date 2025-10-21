#pragma once
#include "Gem.h"

class GatoGem : public Gem {
public:
    GatoGem();
    void setTipoGem(const std::string& tipo) override;
    void onMatch(Board& board, int row, int col) override;
};