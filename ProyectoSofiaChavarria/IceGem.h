#pragma once
#include "Gem.h"

class IceGem : public Gem {
private:
    int hits = 0; // Contador de impactos

public:
    IceGem();
    void setTipoGem(const std::string& tipo) override;
    void onMatch(Board& board, int row, int col) override;
    bool canBeSwapped() const { return false; } // Para bloquear intercambio
    void receiveHit(); // Llamado desde Board cuando una gema adyacente se elimina
};