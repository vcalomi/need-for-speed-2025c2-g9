#pragma once
#include <tuple>
#include <utility>

#include <SDL2pp/SDL2pp.hh>

#include "../world/camera.h"
#include "../world/checkpoint.h"
#include "../world/player.h"

class CheckpointIndicator {
public:
    explicit CheckpointIndicator(SDL2pp::Renderer& renderer): renderer_(renderer) {}
    void Draw(const Camera& camera, const Player& localPlayer, const Checkpoint& activeCp);

private:
    SDL2pp::Renderer& renderer_;
    std::pair<float, float> ComputeDirection(const Player& player,
                                             const Checkpoint& checkpoint) const;
    SDL_FPoint ComputeArrowStart(const Player& player, const Camera& camera, float dx,
                                 float dy) const;
    std::tuple<SDL_FPoint, SDL_FPoint, SDL_FPoint> ComputeTrianglePoints(const SDL_FPoint& start,
                                                                         float dx, float dy) const;
    void DrawFilledTriangle(SDL_FPoint a, SDL_FPoint b, SDL_FPoint c);
};
