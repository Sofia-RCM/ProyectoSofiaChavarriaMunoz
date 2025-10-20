#include "PartiGem.h"
#include "Board.h"
// Definición del constructor
PartiGem::PartiGem() {}

void PartiGem::setTipoGem(const std::string& tipo) {
    Gem::setTipoGem(tipo);
}

void PartiGem::onMatch(Board& board, int row, int col) {
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned>(std::time(0)));
        seeded = true;
    }
    for (int r = 0; r < 8; ++r) {
        int c = std::rand() % 8;
        board.markForClear(r, c);
    }
}