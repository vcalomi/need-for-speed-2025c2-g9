#pragma once
#include <map>
#include <string>

#include "./player.h"

class World {
public:
    void AddPlayer(const std::string& id, const std::string& carType, float x, float y,
                   bool isLocal);
    Player& GetLocalPlayer();
    const std::map<std::string, Player>& GetPlayers() const;
    void UpdateFromServer(const std::string& id, float x, float y, float angle);
    void OnCollision(const std::string& id1, const std::string& id2);

private:
    std::map<std::string, Player> players_;
    std::string localId_;
};
