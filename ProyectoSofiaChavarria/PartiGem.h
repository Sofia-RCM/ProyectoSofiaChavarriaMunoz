#pragma once
#include "Gem.h"

class PartiGem : public Gem {
public:
    PartiGem(); // Declaración del constructor
    void setTipoGem(const std::string& tipo) override;
    virtual void onMatch(Board& board, int row, int col) override;
};