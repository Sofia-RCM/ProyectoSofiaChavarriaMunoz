#include "GalletaGem.h"
#include "Board.h"
// Definición del constructor
GalletaGem::GalletaGem() {}

void GalletaGem::setTipoGem(const std::string& tipo) {
    Gem::setTipoGem(tipo);
}

void GalletaGem::onMatch(Board& board, int row, int col) {
    for (int c = 0; c < 8; ++c)
        board.markForClear(row, c);
}