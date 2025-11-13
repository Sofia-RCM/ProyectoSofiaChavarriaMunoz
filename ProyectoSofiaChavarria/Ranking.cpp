#include "Ranking.h"
#include <fstream>
#include <algorithm>
#include <iostream>

Ranking::Ranking(const std::string& file) : filename(file) { load(); }

void Ranking::load() {
    scores.clear();
    std::ifstream in(filename);
    if (!in.is_open()) return;

    std::string name; int score;
    while (in >> name >> score) scores.push_back({ name, score });
    in.close();
    sort();
}

void Ranking::save() {
    std::ofstream out(filename, std::ios::trunc);
    for (auto& s : scores)
        out << s.name << " " << s.score << std::endl;
    out.close();
}

void Ranking::addScore(const std::string& name, int score) {
    scores.push_back({ name, score });
    sort();
    save();
}

std::vector<PlayerScore> Ranking::getTopScores(int n) const {
    std::vector<PlayerScore> top;
    for (int i = 0; i < n && i < (int)scores.size(); ++i)
        top.push_back(scores[i]);
    return top;
}

void Ranking::sort() {
    std::sort(scores.begin(), scores.end(), [](auto& a, auto& b) {
        return a.score > b.score;
        });
}
<<<<<<< HEAD
=======
std::vector<PlayerScore> Ranking::getScores() const {
    return getTopScores(10); // devuelve los 10 mejores
}
>>>>>>> develop
