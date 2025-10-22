#pragma once
#include "Gem.h"

class IceGem : public Gem {
private:
    int health; // Vida (2 golpes para romperse)

public:
    IceGem();
    virtual ~IceGem();

    void receiveHit();               // Recibe un golpe (reduce vida)
    bool isBroken() const;           // Devuelve true si ya está rota
    void onMatch(Board& board, int row, int col) override; // Reacciona si está en match
};
