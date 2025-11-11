#pragma once
#include <map>
#include <string>
#include <vector>

#include "../../common/common_codes.h"
#include "../events/event.h"
#include "./checkpoint.h"
#include "./player.h"

class World {
public:
    explicit World(EventBus& eventBus);
    void AddPlayer(std::string username, VehicleTipe carType, bool isLocal);
    const Player& GetLocalPlayer() const;
    float GetLocalPlayerX() const;
    float GetLocalPlayerY() const;
    bool HasPlayer(std::string username) const;
    bool HasPlayers() const;
    const std::map<std::string, Player>& GetPlayers() const;
    void UpdateFromServer(std::string username, float x, float y, float angle);
    void OnCollision(std::string username1, std::string username2);
    const Player& GetPlayer(const std::string& username) const;
    const std::vector<Checkpoint>& GetCheckpoints() const;

private:
    std::map<std::string, Player> players_;
    std::string localUsername_;
    std::vector<Checkpoint> checkpoints_;
    EventBus& eventBus_;
};
