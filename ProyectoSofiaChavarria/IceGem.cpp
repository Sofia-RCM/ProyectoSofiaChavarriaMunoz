#include "IceGem.h"
#include "Board.h"

IceGem::IceGem() {}

void IceGem::setTipoGem(const std::string& tipo) {
    Gem::setTipoGem(tipo);
}

void IceGem::onMatch(Board& board, int row, int col) {
    // IceGem no se activa en matches
}

void IceGem::receiveHit() {
    hits++;
    if (hits >= 2) {
        empty = true; // Marcar para eliminación
    }
}