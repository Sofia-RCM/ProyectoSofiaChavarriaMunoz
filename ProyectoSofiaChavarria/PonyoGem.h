#pragma once
#include "Gem.h"

class PonyoGem : public Gem {
public:
    PonyoGem(); // Declaración del constructor
    virtual void onMatch(Board& board, int row, int col) override;
    void setTipoGem(const std::string& tipo) override;
};