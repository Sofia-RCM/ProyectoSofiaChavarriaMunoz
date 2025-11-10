#pragma once
#include <string>
#include <vector>

struct PlayerScore {
    std::string name;
    int score;
};

class Ranking {
    std::string filename;
    std::vector<PlayerScore> scores;
public:
    Ranking(const std::string& file = "ranking.txt");
    void load();
    void save();
    void addScore(const std::string& name, int score);
    std::vector<PlayerScore> getTopScores(int n = 5) const;
private:
    void sort();
};
