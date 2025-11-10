#include "GatoGem.h"
#include "Board.h"

GatoGem::GatoGem() {}
void GatoGem::setTipoGem(const std::string& tipo) { Gem::setTipoGem(tipo); }

void GatoGem::onMatch(Board& board, int row, int col) {
    if (isSpecial && !isActivated) {
        isActivated = true;

        std::vector<std::pair<int, int>> affected;
        // Fila completa donde está la gema
        for (int c = 0; c < 8; ++c) affected.push_back({ row, c });

        // ?? Fila completa
        board.startHighlight(affected, 0.45f, sf::Color(100, 255, 120, 180));
    }
}
