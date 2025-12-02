#include "ranking_calculator.h"
#include "gameloop.h"

#include <algorithm>

std::vector<RankEntry> RankingCalculator::calculateRankings(
    const std::unordered_map<int, PlayerResult>& playerResults,
    const std::map<int, std::string>& playerUsernames) {

    std::vector<RankEntry> entries;
    entries.reserve(playerResults.size());

    for (const auto& [vehicleId, result] : playerResults) {
        auto usernameIt = playerUsernames.find(vehicleId);
        if (usernameIt == playerUsernames.end()) continue;

        RankEntry e{
            vehicleId,
            usernameIt->second,
            result.totalTimeSeconds,
            result.totalPenaltySeconds,
            result.totalTimeSeconds + result.totalPenaltySeconds
        };
        entries.push_back(std::move(e));
    }

    std::sort(entries.begin(), entries.end(), [](const RankEntry& a, const RankEntry& b) {
        if (a.score == b.score) {
            return a.vehicleId < b.vehicleId;
        }
        return a.score < b.score;
    });

    return entries;
}
