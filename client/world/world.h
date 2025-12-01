#pragma once
#include <map>
#include <string>
#include <vector>

#include "../../common/common_codes.h"
#include "./checkpoint.h"
#include "./player.h"

#include "progress_manager.h"

class World {
public:
    explicit World(EventBus& eventBus);

    void AddPlayer(std::string username, VehicleTipe carType, bool isLocal, float health);

    const Player& GetLocalPlayer() const;
    float GetLocalPlayerX() const;
    float GetLocalPlayerY() const;

    bool HasPlayer(const std::string& username) const;
    bool HasPlayers() const;

    const std::map<std::string, Player>& GetPlayers() const;

    void UpdateFromServer(std::string username, float x, float y, float angle, float speed,
                          bool isAboveBridge);

    const Player& GetPlayer(const std::string& username) const;
    Player& GetPlayer(const std::string& username);

    void ResetPlayersExploded();
    void RemovePlayer(const std::string& username);

    void SetCheckpoints(const std::vector<Checkpoint>& checkpoints);
    const std::vector<Checkpoint>& GetCheckpoints() const;

    void OnCollision(const Event& e);

private:
    std::map<std::string, Player> players_;
    std::string localUsername_;
    std::vector<Checkpoint> checkpoints_;
    EventBus& eventBus_;
};
