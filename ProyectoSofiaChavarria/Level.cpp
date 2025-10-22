#include "Level.h"

Level::Level(int num, int moves, bool ice)
    : levelNumber(num), movesLimit(moves), hasIceObstacles(ice) {}

void Level::addObjective(const Objective& obj) {
    objectives.push_back(obj);
}

bool Level::isCompleted() const {
    for (const auto& obj : objectives) {
        if (!obj.isCompleted()) {
            return false;
        }
    }
    return true;
}

const std::vector<Objective>& Level::getObjectives() {
    return objectives;
}

int Level::getMovesLimit() const {
    return movesLimit;
}

int Level::getLevelNumber() const {
    return levelNumber;
}

bool Level::getHasIceObstacles() const {
    return hasIceObstacles;
}