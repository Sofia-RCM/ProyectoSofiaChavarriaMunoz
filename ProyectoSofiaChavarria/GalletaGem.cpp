#include "GalletaGem.h"

GalletaGem::GalletaGem() { setTipoGem("GalletaEspecial"); }

void GalletaGem::onMatch(Board& board, int row, int col) {
    if (isSpecial && !isActivated) {
        isActivated = true;

        static const int dr[8] = { -1,-1,-1, 0, 0, 1, 1, 1 };
        static const int dc[8] = { -1, 0, 1,-1, 1,-1, 0, 1 };
        std::vector<std::pair<int, int>> affected;

        for (int i = 0; i < 8; ++i) {
            int nr = row + dr[i], nc = col + dc[i];
            if (board.isValid(nr, nc))
                affected.push_back({ nr, nc });
        }
        affected.push_back({ row, col });

        board.startHighlight(affected, 0.45f, sf::Color(255, 230, 100, 180));
    }
}
