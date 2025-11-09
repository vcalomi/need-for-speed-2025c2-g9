#pragma once
#include <map>
#include <string>

#include "../../common/common_codes.h"
#include "./player.h"

class World {
public:
    World();
    void AddPlayer(std::string username, VehicleTipe carType, bool isLocal);
    Player& GetLocalPlayer();
    float GetLocalPlayerX() const;
    float GetLocalPlayerY() const;
    bool HasPlayer(std::string username) const;
    bool HasPlayers() const;
    const std::map<std::string, Player>& GetPlayers() const;
    void UpdateFromServer(std::string username, float x, float y, float angle);
    void OnCollision(std::string username1, std::string username2);

private:
    std::map<std::string, Player> players_;
    std::string localUsername_;
};
