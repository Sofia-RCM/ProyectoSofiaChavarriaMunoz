#pragma once
#include "Gem.h"

class GalletaGem : public Gem {
public:
    GalletaGem(); // Declaración del constructor
    void setTipoGem(const std::string& tipo) override;
    virtual void onMatch(Board& board, int row, int col) override;
};