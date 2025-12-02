#ifndef RANKING_CALCULATOR_H
#define RANKING_CALCULATOR_H

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include "gameloop.h"

struct RankEntry {
    int vehicleId;
    std::string username;
    float totalTime;
    float penalties;
    float score;
};

class RankingCalculator {
public:
    static std::vector<RankEntry> calculateRankings(
        const std::unordered_map<int, PlayerResult>& playerResults,
        const std::map<int, std::string>& playerUsernames);
};

#endif
