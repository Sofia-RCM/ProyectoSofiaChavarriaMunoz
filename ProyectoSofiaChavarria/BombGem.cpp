#include "BombGem.h"
#include "Board.h"

BombGem::BombGem() {} // Definición del constructor

void BombGem::setTipoGem(const std::string& tipo) {
    Gem::setTipoGem(tipo);
}

void BombGem::onMatch(Board& board, int row, int col) {
    for (int c = 0; c < 8; ++c) board.markForClear(row, c);
    for (int r = 0; r < 8; ++r) board.markForClear(r, col);
}