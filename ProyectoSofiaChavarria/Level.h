#pragma once
#include <vector>
#include "Objective.h"

class Level {
private:
    int levelNumber;
    std::vector<Objective> objectives;
    int movesLimit;
    bool hasIceObstacles;

public:
    Level(int num, int moves, bool ice = false);
    void addObjective(const Objective& obj);
    bool isCompleted() const;
    const std::vector<Objective>& getObjectives();
    int getMovesLimit() const;
    int getLevelNumber() const;
    bool getHasIceObstacles() const;
};