#include "./world.h"

#include <cmath>

void World::AddPlayer(const std::string& id, const std::string& carType, float x, float y,
                      bool isLocal) {
    players_.emplace(id, Player(id, carType, x, y));
    if (isLocal)
        localId_ = id;
}

Player& World::GetLocalPlayer() { return players_.at(localId_); }

const std::map<std::string, Player>& World::GetPlayers() const { return players_; }

void World::UpdateFromServer(const std::string& id, float x, float y, float angle) {
    auto it = players_.find(id);
    if (it == players_.end()) {
        std::cerr << "[World] Warning: received update for unknown player ID " << id << "\n";
        return;
    }

    Player& player = it->second;

    // Actualizamos la posición y rotación según datos del servidor
    player.UpdateFromNetwork(x, y, angle);
}

void World::OnCollision(const std::string& id1, const std::string& id2) {
    std::cout << "💥 Collision between " << id1 << " and " << id2 << std::endl;
}
