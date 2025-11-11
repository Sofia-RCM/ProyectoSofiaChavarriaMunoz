#pragma once
#include <string>

struct LevelData {
    int nivel = 1;
    int totoroGoal = 0;
    int iceGoal = 0;
    int ponyoGoal = 0;
    int movimientos = 20;
    std::string fondo = "assets/FondoM.png";

    bool loadFromFile(const std::string& filename);
};
