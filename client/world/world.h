#pragma once
#include <map>
#include <string>

#include "./player.h"

class World {
public:
    World();
    void AddPlayer(const int id, const std::string& carType, float x, float y, bool isLocal);
    Player& GetLocalPlayer();
    bool HasPlayer(const int id) const;
    const std::map<int, Player>& GetPlayers() const;
    void UpdateFromServer(const int id, float x, float y, float angle);
    void OnCollision(const int id1, const int id2);

private:
    std::map<int, Player> players_;
    int localId_;
};
