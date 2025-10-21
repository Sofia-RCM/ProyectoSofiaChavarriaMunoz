#include "TotoroGem.h"
#include "Board.h"

TotoroGem::TotoroGem() {}

void TotoroGem::setTipoGem(const std::string& tipo) {
    Gem::setTipoGem(tipo);
}

void TotoroGem::onMatch(Board& board, int row, int col) {
    if (isSpecial && !isActivated) {
        isActivated = true;
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                board.markForClear(r, c);
            }
        }
    }
}