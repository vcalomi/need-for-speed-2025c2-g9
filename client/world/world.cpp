#include "./world.h"

#include <cmath>

#include "../events/checkpoint_event.h"
#include "../events/player_collision_event.h"
#include "../events/player_events.h"
#include "../events/player_joined_event.h"
#include "../events/player_left_event.h"
#include "../events/wall_collision_event.h"

World::World(EventBus& eventBus): eventBus_(eventBus) {
    eventBus_.Subscribe<PlayerJoinedEvent>([this](const PlayerJoinedEvent& e) {
        if (!HasPlayer(e.username)) {
            AddPlayer(e.username, e.type, e.isLocal, e.health);
        }
    });

    eventBus_.Subscribe<PlayerLeftEvent>(
            [this](const PlayerLeftEvent& e) { RemovePlayer(e.username); });

    eventBus_.Subscribe<PlayerStateUpdatedEvent>([this](const PlayerStateUpdatedEvent& e) {
        UpdateFromServer(e.username, e.x, e.y, e.angle, e.speed, e.isAboveBridge);
    });

    eventBus_.Subscribe<PlayerCollisionEvent>(
            [this](const PlayerCollisionEvent& e) { OnCollision(e); });

    eventBus_.Subscribe<WallCollisionEvent>(
            [this](const WallCollisionEvent& e) { OnCollision(e); });

    eventBus_.Subscribe<CheckPointEvent>([this](const CheckPointEvent& e) {
        Checkpoint cp;
        cp.id = e.id;
        cp.x = e.x;
        cp.y = e.y;

        checkpoints_.push_back(cp);

        std::sort(checkpoints_.begin(), checkpoints_.end(),
                  [](const Checkpoint& a, const Checkpoint& b) { return a.id < b.id; });
    });
}

void World::AddPlayer(std::string username, VehicleTipe carType, bool isLocal, float health) {
    float spawnDistance = 3.0f;
    float angle = 0.5f;
    float defaultX = std::cos(angle) * spawnDistance;
    float defaultY = std::sin(angle) * spawnDistance;

    players_.emplace(username, Player(username, carType, defaultX, defaultY, health));

    if (isLocal)
        localUsername_ = username;
}

void World::RemovePlayer(const std::string& username) { players_.erase(username); }

bool World::HasPlayers() const { return !players_.empty(); }

const Player& World::GetLocalPlayer() const { return players_.at(localUsername_); }

const std::map<std::string, Player>& World::GetPlayers() const { return players_; }

bool World::HasPlayer(const std::string& username) const {
    return players_.find(username) != players_.end();
}

void World::UpdateFromServer(std::string username, float x, float y, float angle, float speed,
                             bool isAboveBridge) {
    auto it = players_.find(username);
    if (it == players_.end()) {
        std::cerr << "[World] Warning: update for unknown player " << username << "\n";
        return;
    }
    it->second.UpdateFromNetwork(x, y, angle, speed, isAboveBridge);
}

void World::ResetPlayersExploded() {
    for (auto& [username, player]: players_) {
        player.setExploded(false);
    }
}

void World::OnCollision(const Event& e) {
    if (e.GetType() == PlayerCollisionEvent::Type()) {
        const auto& ev = static_cast<const PlayerCollisionEvent&>(e);

        auto it1 = players_.find(ev.player1_username);
        if (it1 != players_.end())
            it1->second.updateHealth(ev.player1_new_hp);

        auto it2 = players_.find(ev.player2_username);
        if (it2 != players_.end())
            it2->second.updateHealth(ev.player2_new_hp);

    } else if (e.GetType() == WallCollisionEvent::Type()) {
        const auto& ev = static_cast<const WallCollisionEvent&>(e);

        auto it = players_.find(ev.player1_username);
        if (it != players_.end())
            it->second.updateHealth(ev.player1_new_hp);
    }
}


float World::GetLocalPlayerX() const {
    auto it = players_.find(localUsername_);
    return (it != players_.end()) ? it->second.GetX() : 0.0f;
}

float World::GetLocalPlayerY() const {
    auto it = players_.find(localUsername_);
    return (it != players_.end()) ? it->second.GetY() : 0.0f;
}

Player& World::GetPlayer(const std::string& username) { return players_.at(username); }

const Player& World::GetPlayer(const std::string& username) const { return players_.at(username); }

const std::vector<Checkpoint>& World::GetCheckpoints() const { return checkpoints_; }

void World::SetCheckpoints(const std::vector<Checkpoint>& checkpoints) {
    checkpoints_ = checkpoints;
}
