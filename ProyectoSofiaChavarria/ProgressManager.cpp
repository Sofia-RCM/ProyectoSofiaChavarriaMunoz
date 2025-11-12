#include "ProgressManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>

static std::string toLowerCopy(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(),
        [](unsigned char c) { return (char)std::tolower(c); });
    return out;
}

ProgressManager::ProgressManager(const std::string& filename) : file(filename) {
    load();
}

std::string ProgressManager::normalize(const std::string& s) {
    // nombre en minúsculas, sin espacios extremos
    std::string t = s;
    // recorte simple
    while (!t.empty() && (t.back() == ' ' || t.back() == '\t' || t.back() == '\n' || t.back() == '\r')) t.pop_back();
    size_t i = 0; while (i < t.size() && (t[i] == ' ' || t[i] == '\t' || t[i] == '\n' || t[i] == '\r')) ++i;
    t = t.substr(i);
    return toLowerCopy(t);
}

void ProgressManager::load() {
    data.clear();
    std::ifstream in(file);
    if (!in.is_open()) return; // si no existe, bien: se crea al guardar
    std::string name;
    int lvl;
    while (in >> name >> lvl) {
        data[normalize(name)] = std::max(1, std::min(lvl, 3));
    }
    in.close();
}

void ProgressManager::save() const {
    std::ofstream out(file, std::ios::trunc);
    if (!out.is_open()) return;
    for (const auto& kv : data) {
        out << kv.first << " " << kv.second << "\n";
    }
    out.close();
}

int ProgressManager::getLevel(const std::string& playerName) const {
    auto key = normalize(playerName);
    auto it = data.find(key);
    if (it == data.end()) return 1;
    return it->second;
}

void ProgressManager::setLevelUnlocked(const std::string& playerName, int maxUnlocked) {
    maxUnlocked = std::max(1, std::min(maxUnlocked, 3));
    auto key = normalize(playerName);
    auto it = data.find(key);
    if (it == data.end()) {
        data[key] = maxUnlocked;
    }
    else {
        if (maxUnlocked > it->second) it->second = maxUnlocked; // nunca reducir
    }
    save();
}

void ProgressManager::reload() {
    load();
}
