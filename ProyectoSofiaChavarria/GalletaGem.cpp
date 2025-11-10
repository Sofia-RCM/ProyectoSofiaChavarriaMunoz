#include "GalletaGem.h"
#include "Board.h"

GalletaGem::GalletaGem() {}
void GalletaGem::setTipoGem(const std::string& tipo) { Gem::setTipoGem(tipo); }

void GalletaGem::onMatch(Board& board, int row, int col) {
    if (isSpecial && !isActivated) {
        isActivated = true;

        // 8 vecinos (cruz + diagonales) alrededor de (row,col)
        static const int dr[8] = { -1,-1,-1, 0, 0, 1, 1, 1 };
        static const int dc[8] = { -1, 0, 1,-1, 1,-1, 0, 1 };

        std::vector<std::pair<int, int>> affected;
        for (int i = 0; i < 8; ++i) {
            int nr = row + dr[i], nc = col + dc[i];
            if (board.isValid(nr, nc))
                affected.push_back({ nr, nc });
        }
        // (opcional) incluir la propia celda:
        affected.push_back({ row, col });

        // ?? Vecindad de 8
        board.startHighlight(affected, 0.45f, sf::Color(100, 255, 120, 180));
    }
}
