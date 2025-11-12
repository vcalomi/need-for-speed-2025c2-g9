#include "./world.h"

#include <cmath>

#include "../events/checkpoint_event.h"
#include "../events/player_events.h"
#include "../events/player_joined_event.h"

World::World(EventBus& eventBus): eventBus_(eventBus) {
    eventBus_.Subscribe<CheckPointEvent>([this](const CheckPointEvent& e) {
        Checkpoint newCheckpoint;
        newCheckpoint.id = e.id;
        newCheckpoint.x = e.x;
        newCheckpoint.y = e.y;
        checkpoints_.push_back(newCheckpoint);
    });

    eventBus_.Subscribe<PlayerJoinedEvent>([this](const PlayerJoinedEvent& e) {
        if (!HasPlayer(e.username)) {
            AddPlayer(e.username, e.type, e.isLocal);
        }
    });

    eventBus_.Subscribe<PlayerStateUpdatedEvent>([this](const PlayerStateUpdatedEvent& e) {
        UpdateFromServer(e.username, e.x, e.y, e.angle);
    });
}

void World::AddPlayer(std::string username, VehicleTipe carType, bool isLocal) {
    float spawnDistance = 3.0f;
    float angle = 0.5f;
    float defaultX = std::cos(angle) * spawnDistance;
    float defaultY = std::sin(angle) * spawnDistance;
    players_.emplace(username, Player(username, carType, defaultX, defaultY));
    if (isLocal)
        localUsername_ = username;
}

bool World::HasPlayers() const { return !players_.empty(); }

const Player& World::GetLocalPlayer() const { return players_.at(localUsername_); }

const std::map<std::string, Player>& World::GetPlayers() const { return players_; }

bool World::HasPlayer(std::string username) const {
    return players_.find(username) != players_.end();
}

void World::UpdateFromServer(std::string username, float x, float y, float angle) {
    auto it = players_.find(username);
    if (it == players_.end()) {
        std::cerr << "[World] Warning: received update for unknown player " << username << "\n";
        return;
    }

    Player& player = it->second;

    // Actualizamos la posición y rotación según datos del servidor
    player.UpdateFromNetwork(x, y, angle);
}

void World::OnCollision(std::string username1, std::string username2) {
    std::cout << "💥 Collision between " << username1 << " and " << username2 << std::endl;
}

float World::GetLocalPlayerX() const {
    auto it = players_.find(localUsername_);
    if (it != players_.end()) {
        return it->second.GetX();
    }
    return 0.0f;  // Valor por defecto si no se encuentra el jugador local
}

float World::GetLocalPlayerY() const {
    auto it = players_.find(localUsername_);
    if (it != players_.end()) {
        return it->second.GetY();
    }
    return 0.0f;  // Valor por defecto si no se encuentra el jugador local
}

const Player& World::GetPlayer(const std::string& username) const { return players_.at(username); }

const std::vector<Checkpoint>& World::GetCheckpoints() const { return checkpoints_; }
