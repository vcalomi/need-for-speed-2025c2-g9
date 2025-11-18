#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>

#include "../../common/common_codes.h"
#include "../events/event.h"
#include "./checkpoint.h"
#include "./player.h"

struct PlayerProgress {
    int currentCheckpointIndex = 0;
    int lapsCompleted = 0;
};

class World {
public:
    explicit World(EventBus& eventBus);
    void AddPlayer(std::string username, VehicleTipe carType, bool isLocal, float health);
    const Player& GetLocalPlayer() const;
    float GetLocalPlayerX() const;
    float GetLocalPlayerY() const;
    bool HasPlayer(std::string username) const;
    bool HasPlayers() const;
    const std::map<std::string, Player>& GetPlayers() const;
    void UpdateFromServer(std::string username, float x, float y, float angle, float speed,
                          bool isAboveBridge);
    void OnCollision(const Event& e);
    const Player& GetPlayer(const std::string& username) const;


    void SetCheckpoints(const std::vector<Checkpoint>& checkpoints);
    const std::vector<Checkpoint>& GetCheckpoints() const;
    const Checkpoint& GetActiveCheckpointFor(const std::string& username) const;
    const Checkpoint& GetNextCheckpointFor(const std::string& username) const;
    void OnPlayerReachedCheckpoint(const std::string& username, int checkpointId);
    const std::set<int> GetPassedCheckpointIdsFor(const std::string& username) const;

    int GetLapsFor(const std::string& username) const;
    int GetLapProgressFor(const std::string& username) const;

    void resetRace();

private:
    std::map<std::string, Player> players_;
    std::string localUsername_;
    std::vector<Checkpoint> checkpoints_;
    EventBus& eventBus_;
    std::map<std::string, PlayerProgress> playerProgress_;
};
