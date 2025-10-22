#pragma once
#include <string>

enum class ObjectiveType {
    ELIMINATE_GEM,
    CLEAR_OBSTACLES,
    SCORE_TARGET
};

class Objective {
private:
    ObjectiveType type;
    std::string gemType; // solo usado si type == ELIMINATE_GEM
    int target;
    int current;

public:
    Objective(ObjectiveType t, int tgt, const std::string& gt = "");
    void increment(int amount = 1);
    bool isCompleted() const;
    int getProgress() const;
    int getTarget() const;
    ObjectiveType getType() const;
    std::string getGemType() const;
    std::string getDescription() const;
};