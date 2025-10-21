#pragma once
#include "Gem.h"

class GalletaGem : public Gem {
public:
    GalletaGem();
    void setTipoGem(const std::string& tipo) override;
    void onMatch(Board& board, int row, int col) override;
};