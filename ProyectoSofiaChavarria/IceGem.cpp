#include "IceGem.h"
#include "Board.h"
#include <iostream>
using namespace std;

IceGem::IceGem() : health(2) {
    setTipoGem("Ice");
}

IceGem::~IceGem() {}

void IceGem::receiveHit() {
    if (empty) return;  // Si ya está rota, no hace nada
    health--;
   
    if (health <= 0) {
        empty = true;
    }
}

bool IceGem::isBroken() const {
    return empty;
}

void IceGem::onMatch(Board& board, int row, int col) {
    // Si una IceGem está dentro de un match, también se daña
    receiveHit();
}
