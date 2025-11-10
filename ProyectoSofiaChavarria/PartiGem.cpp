#include "PartiGem.h"
#include "Board.h"

PartiGem::PartiGem() {}
void PartiGem::setTipoGem(const std::string& tipo) { Gem::setTipoGem(tipo); }

void PartiGem::onMatch(Board& board, int row, int col) {
    if (isSpecial && !isActivated) {
        isActivated = true;
        std::vector<std::pair<int, int>> affected;

        for (int dr = -1; dr <= 1; ++dr) {
            int r = row + dr;
            if (board.isValid(r, 0))
                for (int c = 0; c < 8; ++c) affected.push_back({ r, c });
        }
        for (int dc = -1; dc <= 1; ++dc) {
            int c = col + dc;
            if (board.isValid(0, c))
                for (int r = 0; r < 8; ++r) affected.push_back({ r, c });
        }

        board.startHighlight(affected, 0.45f, sf::Color(100, 255, 120, 180));
    }
}
