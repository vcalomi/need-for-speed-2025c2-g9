#pragma once
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../common/common_codes.h"
#include "./checkpoint.h"
#include "./player.h"

#include "progress_manager.h"

struct Npc {
    int id;
    float x;
    float y;
    std::string spriteName;
};

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
                          float health, bool isAboveBridge);

    const Player& GetPlayer(const std::string& username) const;
    Player& GetPlayer(const std::string& username);

    void ResetPlayersExploded();
    void RemovePlayer(const std::string& username);

    void SetCheckpoints(const std::vector<Checkpoint>& checkpoints);
    const std::vector<Checkpoint>& GetCheckpoints() const;

    void OnCollision(const Event& e);
    const auto& GetNpcs() const { return npcs_; }
    void AddNpc(const int id, float x, float y, const std::string& sprite) {
        npcs_[id] = Npc{id, x, y, sprite};
    }
    void RemoveNpcById(int npcId) {
        auto it = npcs_.find(npcId);
        if (it != npcs_.end()) {
            npcs_.erase(it);
        }
    }


private:
    std::map<std::string, Player> players_;
    std::string localUsername_;
    std::vector<Checkpoint> checkpoints_;
    EventBus& eventBus_;
    std::unordered_map<int, Npc> npcs_;
};
