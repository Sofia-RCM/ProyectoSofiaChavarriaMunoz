#pragma once
#include "Gem.h"

class NormalGem : public Gem {
public:
    NormalGem();
    void setTipoGem(const std::string& tipo) override;
};