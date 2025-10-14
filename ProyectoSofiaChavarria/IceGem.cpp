#include "IceGem.h"
#include <iostream>
using namespace std;

IceGem::IceGem() : Gem(), hitsRemaining(2) {}

void IceGem::onMatch() {
    hitsRemaining--;
    if (hitsRemaining <= 0) {
        tipoGem.clear();
        loaded = false;
    }
    else {
    }
}
