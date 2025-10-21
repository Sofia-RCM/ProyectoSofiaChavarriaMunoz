#pragma once
#include <string>

const int OBJ_ELIMINATE_GEM = 0;
const int OBJ_CLEAR_OBSTACLES = 1;
const int OBJ_SCORE_TARGET = 2;

class Objective {
public:
    int type;
    std::string gemType;
    int target;
    int current;

    Objective(int t, int targetAmount, const std::string& gem = "")
        : type(t), target(targetAmount), current(0), gemType(gem) {}

    bool isCompleted() const {
        return current >= target;
    }

    void increment(int amount = 1) {
        current += amount;
    }
};
