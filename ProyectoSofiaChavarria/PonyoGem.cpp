#include "PonyoGem.h"
#include "Board.h"
// Definición del constructor
PonyoGem::PonyoGem() {}

void PonyoGem::setTipoGem(const std::string& tipo) {
    Gem::setTipoGem(tipo);
}

void PonyoGem::onMatch(Board& board, int row, int col) {
    for (int c = 0; c < 8; ++c) board.markForClear(row, c);
    for (int r = 0; r < 8; ++r) board.markForClear(r, col);
}