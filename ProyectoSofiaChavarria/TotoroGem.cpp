#include "TotoroGem.h"
#include "Board.h"

TotoroGem::TotoroGem() {}

void TotoroGem::setTipoGem(const std::string& tipo) {
    Gem::setTipoGem(tipo);
}
void TotoroGem::onMatch(Board& board, int row, int col) {
    if (isSpecial && !isActivated) {
        isActivated = true;
        std::vector<std::pair<int, int>> affected;
        for (int c = 0; c < 8; ++c) affected.push_back({ row, c });
        for (int r = 0; r < 8; ++r) affected.push_back({ r, col });
        board.startHighlight(affected, 0.45f, sf::Color(100, 255, 120, 180));
    }
}