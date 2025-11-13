#pragma once
#include <string>
#include <unordered_map>

class ProgressManager {
public:
    // Carga el archivo progress.txt al iniciar
    ProgressManager(const std::string& filename = "progress.txt");

    // Devuelve el mayor nivel desbloqueado para un jugador (1..3). Si no existe, retorna 1.
    int getLevel(const std::string& playerName) const;

    // Establece (y guarda) el mayor nivel desbloqueado para el jugador
    // Si el valor es menor que el ya guardado, no reduce el progreso.
    void setLevelUnlocked(const std::string& playerName, int maxUnlocked);

    // Fuerza recarga desde disco (por si editas el archivo a mano)
    void reload();

private:
    std::string file;
    std::unordered_map<std::string, int> data; // nombre(minúsculas) -> nivel max desbloqueado

    static std::string normalize(const std::string& s);
    void load();
    void save() const;
};
