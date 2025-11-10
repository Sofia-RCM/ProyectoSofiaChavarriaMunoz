#include "GatoGem.h"
#include "Board.h"

GatoGem::GatoGem() {}
void GatoGem::setTipoGem(const std::string& tipo) { Gem::setTipoGem(tipo); }

void GatoGem::onMatch(Board& board, int row, int col) {
    if (isSpecial && !isActivated) {
        isActivated = true;
        int dx[8] = { -1,-1,-1,0,0,1,1,1 };
        int dy[8] = { -1,0,1,-1,1,-1,0,1 };
        std::vector<std::pair<int, int>> affected;
        for (int i = 0; i < 8; ++i) {
            int nr = row + dx[i], nc = col + dy[i];
            if (board.isValid(nr, nc)) affected.push_back({ nr, nc });
        }
        affected.push_back({ row, col });
        board.startHighlight(affected, 0.45f, sf::Color(100, 255, 120, 180));
    }
}
