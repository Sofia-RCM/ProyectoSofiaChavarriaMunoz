#pragma once
#include <vector>
#include "Objective.h"

class Level {
public:
    int moves;
    int levelNumber;
    std::vector<Objective> objectives;
    int movesLimit;
    bool hasIceObstacles;

    Level(int num, int moves, bool ice = false)
        : levelNumber(num), movesLimit(moves), hasIceObstacles(ice) {}

    void addObjective(const Objective& obj) {
        objectives.push_back(obj);
    }

    bool isCompleted() const {
        for (const auto& obj : objectives) {
            if (!obj.isCompleted()) {
                return false;
            }
        }
        return true;
    }

    bool hasIce() const {
        return hasIceObstacles;
    }




};