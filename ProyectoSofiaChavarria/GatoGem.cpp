#include "GatoGem.h"
#include "Board.h"
// Definición del constructor
GatoGem::GatoGem() {}

void GatoGem::setTipoGem(const std::string& tipo) {
    Gem::setTipoGem(tipo);
}

void GatoGem::onMatch(Board& board, int row, int col) {
    int dx[8] = { -1,-1,-1,0,0,1,1,1 };
    int dy[8] = { -1,0,1,-1,1,-1,0,1 };
    for (int i = 0; i < 8; ++i) {
        int nr = row + dx[i], nc = col + dy[i];
        if (board.isValid(nr, nc))
            board.markForClear(nr, nc);
    }
}