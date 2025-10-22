#include "Objective.h"
#include <string>

Objective::Objective(ObjectiveType t, int tgt, const std::string& gt)
    : type(t), gemType(gt), target(tgt), current(0) {}

void Objective::increment(int amount) {
    current += amount;
    if (current > target) {
        current = target;
    }
}

bool Objective::isCompleted() const {
    return current >= target;
}

int Objective::getProgress() const {
    return current;
}

int Objective::getTarget() const {
    return target;
}

ObjectiveType Objective::getType() const {
    return type;
}

std::string Objective::getGemType() const {
    return gemType;
}

std::string Objective::getDescription() const {
    switch (type) {
    case ObjectiveType::ELIMINATE_GEM:
        return "Elimina " + std::to_string(target) + " gemas " + gemType;
    case ObjectiveType::CLEAR_OBSTACLES:
        return "Rompe " + std::to_string(target) + " bloques de hielo";
    case ObjectiveType::SCORE_TARGET:
        return "Alcanza " + std::to_string(target) + " puntos";
    default:
        return "Objetivo";
    }
}