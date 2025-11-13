#pragma once
#include <string>

<<<<<<< HEAD
struct LevelData {
    int nivel = 1;
    int totoroGoal = 0;
    int iceGoal = 0;
    int ponyoGoal = 0;
    int movimientos = 20;
    std::string fondo = "assets/FondoM.png";

    bool loadFromFile(const std::string& filename);
=======
class LevelData {
public:
    LevelData();
    bool loadFromFile(const std::string& filename);

    int getMoves() const;
    int getGoalTotoro() const;
    int getGoalIce() const;
    int getGoalPonyo() const;
    std::string getBackground() const;

private:
    int moves;
    int goalTotoro;
    int goalIce;
    int goalPonyo;
    std::string background;
>>>>>>> develop
};
