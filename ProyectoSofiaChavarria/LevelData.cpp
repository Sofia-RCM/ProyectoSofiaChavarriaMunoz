#include "LevelData.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool LevelData::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cerr << " No se pudo abrir el archivo de nivel : " << filename << std::endl;
        return false;
    }

    std::string line;
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
