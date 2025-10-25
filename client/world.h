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
    void Update(float delta);

private:
    std::map<std::string, Player> players_;
    std::string localId_;
};
