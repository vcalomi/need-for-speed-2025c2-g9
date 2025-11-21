#include "./world.h"

#include <cmath>

#include "../events/checkpoint_completed_event.h"
#include "../events/checkpoint_event.h"
#include "../events/player_collision_event.h"
#include "../events/player_events.h"
#include "../events/player_joined_event.h"
#include "../events/player_race_finished_event.h"
#include "../events/race_finished_event.h"
#include "../events/wall_collision_event.h"

World::World(EventBus& eventBus): eventBus_(eventBus) {
    eventBus_.Subscribe<CheckPointEvent>([this](const CheckPointEvent& e) {
        Checkpoint newCheckpoint;
        newCheckpoint.id = e.id;
        newCheckpoint.x = e.x;
        newCheckpoint.y = e.y;
        checkpoints_.push_back(newCheckpoint);

        std::sort(checkpoints_.begin(), checkpoints_.end(),
                  [](const Checkpoint& a, const Checkpoint& b) { return a.id < b.id; });
    });

    eventBus_.Subscribe<PlayerJoinedEvent>([this](const PlayerJoinedEvent& e) {
        if (!HasPlayer(e.username)) {
            AddPlayer(e.username, e.type, e.isLocal, e.health);
        }
    });

    eventBus_.Subscribe<PlayerStateUpdatedEvent>([this](const PlayerStateUpdatedEvent& e) {
        UpdateFromServer(e.username, e.x, e.y, e.angle, e.speed, e.isAboveBridge);
    });

    eventBus_.Subscribe<CheckPointCompletedEvent>([this](const CheckPointCompletedEvent& e) {
        OnPlayerReachedCheckpoint(e.username, e.checkpoint_id);
    });

    eventBus_.Subscribe<PlayerCollisionEvent>(
            [this](const PlayerCollisionEvent& e) { OnCollision(e); });

    eventBus_.Subscribe<WallCollisionEvent>(
            [this](const WallCollisionEvent& e) { OnCollision(e); });

    eventBus_.Subscribe<RaceFinishedEvent>([this](const RaceFinishedEvent&) { resetRace(); });
}

void World::AddPlayer(std::string username, VehicleTipe carType, bool isLocal, float health) {
    float spawnDistance = 3.0f;
    float angle = 0.5f;
    float defaultX = std::cos(angle) * spawnDistance;
    float defaultY = std::sin(angle) * spawnDistance;
    players_.emplace(username, Player(username, carType, defaultX, defaultY, health));
    playerProgress_[username] = PlayerProgress();
    if (isLocal)
        localUsername_ = username;
}

bool World::HasPlayers() const { return !players_.empty(); }

const Player& World::GetLocalPlayer() const { return players_.at(localUsername_); }

const std::map<std::string, Player>& World::GetPlayers() const { return players_; }

bool World::HasPlayer(std::string username) const {
    return players_.find(username) != players_.end();
}

void World::UpdateFromServer(std::string username, float x, float y, float angle, float speed,
                             bool isAboveBridge) {
    auto it = players_.find(username);
    if (it == players_.end()) {
        std::cerr << "[World] Warning: received update for unknown player " << username << "\n";
        return;
    }

    Player& player = it->second;

    // Actualizamos la posición y rotación según datos del servidor
    player.UpdateFromNetwork(x, y, angle, speed, isAboveBridge);
}

void World::ResetPlayersExploded() {
    for (auto& [username, player]: players_) {
        player.setExploded(false);
    }
}


void World::OnCollision(const Event& e) {
    if (e.GetType() == PlayerCollisionEvent::Type()) {
        const PlayerCollisionEvent& collisionEvent = static_cast<const PlayerCollisionEvent&>(e);
        auto it1 = players_.find(collisionEvent.player1_username);
        if (it1 != players_.end()) {
            it1->second.updateHealth(collisionEvent.player1_new_hp);
        }
        auto it2 = players_.find(collisionEvent.player2_username);
        if (it2 != players_.end()) {
            it2->second.updateHealth(collisionEvent.player2_new_hp);
        }
    } else if (e.GetType() == WallCollisionEvent::Type()) {
        const WallCollisionEvent& collisionEvent = static_cast<const WallCollisionEvent&>(e);
        auto it = players_.find(collisionEvent.player1_username);
        if (it != players_.end()) {
            it->second.updateHealth(collisionEvent.player1_new_hp);
        }
    }
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

Player& World::GetPlayer(const std::string& username) { return players_.at(username); }

const Player& World::GetPlayer(const std::string& username) const { return players_.at(username); }

const std::vector<Checkpoint>& World::GetCheckpoints() const { return checkpoints_; }

void World::SetCheckpoints(const std::vector<Checkpoint>& checkpoints) {
    checkpoints_ = checkpoints;
}

const Checkpoint& World::GetActiveCheckpointFor(const std::string& username) const {
    return checkpoints_.at(playerProgress_.at(username).currentCheckpointIndex);
}

const Checkpoint& World::GetNextCheckpointFor(const std::string& username) const {
    const auto& progress = playerProgress_.at(username);
    int next = (progress.currentCheckpointIndex + 1) % checkpoints_.size();
    return checkpoints_.at(next);
}

void World::OnPlayerReachedCheckpoint(const std::string& username, int checkpointId) {
    if (checkpoints_.empty())
        return;

    auto& progress = playerProgress_[username];
    const auto& current = checkpoints_.at(progress.currentCheckpointIndex);

    if (checkpointId != current.id)
        return;

    progress.currentCheckpointIndex++;

    if (progress.currentCheckpointIndex >= checkpoints_.size()) {
        progress.currentCheckpointIndex = 0;
        progress.lapsCompleted++;
    }
}


const std::set<int> World::GetPassedCheckpointIdsFor(const std::string& username) const {
    std::set<int> passed;

    auto it = playerProgress_.find(username);
    if (it == playerProgress_.end() || checkpoints_.empty())
        return passed;

    const auto& progress = it->second;

    for (int i = 0; i < progress.currentCheckpointIndex && i < (int)checkpoints_.size(); ++i) {
        passed.insert(checkpoints_[i].id);
    }

    return passed;
}

int World::GetLapsFor(const std::string& username) const {
    if (!playerProgress_.count(username))
        return 0;
    return playerProgress_.at(username).lapsCompleted;
}

int World::GetLapProgressFor(const std::string& username) const {
    if (!playerProgress_.count(username))
        return 0;
    return playerProgress_.at(username).currentCheckpointIndex;
}

void World::resetRace() {
    for (auto& [username, progress]: playerProgress_) {
        progress.currentCheckpointIndex = 0;
        progress.lapsCompleted = 0;
    }
}
