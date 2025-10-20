#include "TotoroGem.h"
#include "Board.h"
// Definición del constructor
TotoroGem::TotoroGem() {}

void TotoroGem::setTipoGem(const std::string& tipo) {
    Gem::setTipoGem(tipo);
}
void TotoroGem::onMatch(Board& board, int row, int col) {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            if (board.getGem(r, c) && board.getGem(r, c)->getTipoGem() == "Totoro")
                board.markForClear(r, c);
}