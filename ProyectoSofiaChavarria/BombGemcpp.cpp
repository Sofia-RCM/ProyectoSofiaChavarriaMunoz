#include "BombGem.h"
#include "Board.h"

void BombGem::onMatch(Board& board) {
    int rowToClear = row;
    for (int c = 0; c < 8; c++) {
        board.removeGem(rowToClear, c);
    }
}
