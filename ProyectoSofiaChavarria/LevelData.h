#pragma once
#include <string>

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
};
