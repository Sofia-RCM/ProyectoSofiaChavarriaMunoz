#include "GatoGem.h"

GatoGem::GatoGem() { setTipoGem("GatoEspecial"); }

void GatoGem::onMatch(Board& board, int row, int col) {
    if (isSpecial && !isActivated) {
        isActivated = true;
<<<<<<< HEAD

        std::vector<std::pair<int, int>> affected;
        // Fila completa donde está la gema
        for (int c = 0; c < 8; ++c) affected.push_back({ row, c });

        // ?? Fila completa
        board.startHighlight(affected, 0.45f, sf::Color(100, 255, 120, 180));
=======
        std::vector<std::pair<int, int>> affected;
        for (int c = 0; c < 8; ++c)
            affected.push_back({ row, c });
        board.startHighlight(affected, 0.45f, sf::Color(100, 200, 255, 180));
>>>>>>> develop
    }
}
