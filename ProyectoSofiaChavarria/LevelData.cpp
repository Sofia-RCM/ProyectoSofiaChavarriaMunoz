<<<<<<< HEAD
#include "LevelData.h"
=======
﻿#include "LevelData.h"
>>>>>>> develop
#include <fstream>
#include <sstream>
#include <iostream>

<<<<<<< HEAD
bool LevelData::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cerr << " No se pudo abrir el archivo de nivel : " << filename << std::endl;
=======
LevelData::LevelData() {
    // Valores por defecto por si no se encuentra el archivo
    moves = 20;
    goalTotoro = 30;
    goalIce = 10;
    goalPonyo = 20;
    background = "assets/FondoM.png";
}

bool LevelData::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "⚠️ No se pudo abrir el archivo del nivel: " << filename << std::endl;
>>>>>>> develop
        return false;
    }

    std::string line;
<<<<<<< HEAD
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            if (key == "nivel") nivel = std::stoi(value);
            else if (key == "totoroGoal") totoroGoal = std::stoi(value);
            else if (key == "iceGoal") iceGoal = std::stoi(value);
            else if (key == "ponyoGoal") ponyoGoal = std::stoi(value);
            else if (key == "movimientos") movimientos = std::stoi(value);
            else if (key == "fondo") fondo = value;
        }
    }

    in.close();
    std::cout << "Nivel cargado correctamente: " << filename << std::endl;
    return true;
}
=======
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // Saltar comentarios

        std::istringstream iss(line);
        std::string key;
        if (std::getline(iss, key, '=')) {
            std::string value;
            if (std::getline(iss, value)) {
                if (key == "moves") moves = std::stoi(value);
                else if (key == "goal_totoro") goalTotoro = std::stoi(value);
                else if (key == "goal_ice") goalIce = std::stoi(value);
                else if (key == "goal_ponyo") goalPonyo = std::stoi(value);
                else if (key == "background") background = value;
            }
        }
    }

    file.close();
    std::cout << "✅ Nivel cargado correctamente: " << filename << std::endl;
    return true;
}

int LevelData::getMoves() const { return moves; }
int LevelData::getGoalTotoro() const { return goalTotoro; }
int LevelData::getGoalIce() const { return goalIce; }
int LevelData::getGoalPonyo() const { return goalPonyo; }
std::string LevelData::getBackground() const { return background; }
>>>>>>> develop
