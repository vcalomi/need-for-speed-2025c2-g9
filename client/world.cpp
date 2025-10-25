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

void World::Update(float delta) {
    for (auto& [id, p]: players_) {
        if (id != localId_) {
            // 🔹 Simulación de movimiento remoto
            float newX = p.GetX() + sin(SDL_GetTicks() / 500.0f) * delta * 50;
            float newAngle = fmod(p.GetAngle() + 30.0f * delta, 360.0f);
            p.UpdateFromNetwork(newX, p.GetY(), newAngle, 100.0f);
        }
        p.Update(delta);
    }
}
