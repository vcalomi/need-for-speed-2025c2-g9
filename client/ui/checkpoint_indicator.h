#pragma once
#include <string>
#include <tuple>
#include <utility>

#include <SDL2pp/SDL2pp.hh>

#include "../world/camera.h"
#include "../world/checkpoint.h"
#include "../world/player.h"

class CheckpointIndicator {
public:
    explicit CheckpointIndicator(SDL2pp::Renderer& renderer);

    void SetTexture(const std::string& path);

    void Draw(const Camera& camera, const Player& localPlayer, const Checkpoint& activeCp);

private:
    SDL2pp::Renderer& renderer_;
    SDL2pp::Texture* arrowTexture_ = nullptr;

    std::pair<float, float> ComputeDirection(const Player& player,
                                             const Checkpoint& checkpoint) const;

    float ComputeAngle(float dx, float dy) const;
};
