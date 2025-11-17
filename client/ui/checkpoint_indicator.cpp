#include "checkpoint_indicator.h"

#include <cmath>

CheckpointIndicator::CheckpointIndicator(SDL2pp::Renderer& renderer): renderer_(renderer) {}

void CheckpointIndicator::SetTexture(const std::string& path) {
    SDL2pp::Surface surf(path);
    arrowTexture_ = new SDL2pp::Texture(renderer_, surf);
}

void CheckpointIndicator::Draw(const Camera& camera, const Player& localPlayer,
                               const Checkpoint& activeCp) {
    if (!arrowTexture_)
        return;

    auto [dx, dy] = ComputeDirection(localPlayer, activeCp);
    if (dx == 0.0f && dy == 0.0f)
        return;

    float angleDeg = ComputeAngle(dx, dy);

    float screenX = localPlayer.GetX() - camera.getX();
    float screenY = localPlayer.GetY() - camera.getY();

    const float offsetForward = 50.0f;
    float arrowX = screenX + dx * offsetForward;
    float arrowY = screenY + dy * offsetForward;

    SDL_Rect dst;
    dst.w = 48;
    dst.h = 48;
    dst.x = static_cast<int>(arrowX - dst.w / 2);
    dst.y = static_cast<int>(arrowY - dst.h / 2);

    SDL_RenderCopyEx(renderer_.Get(), arrowTexture_->Get(), nullptr, &dst, angleDeg, nullptr,
                     SDL_FLIP_NONE);
}

std::pair<float, float> CheckpointIndicator::ComputeDirection(const Player& player,
                                                              const Checkpoint& checkpoint) const {
    float dx = checkpoint.x - player.GetX();
    float dy = checkpoint.y - player.GetY();
    float len = std::sqrt(dx * dx + dy * dy);

    if (len < 0.001f)
        return {0.f, 0.f};

    return {dx / len, dy / len};
}

float CheckpointIndicator::ComputeAngle(float dx, float dy) const {
    return std::atan2(dy, dx) * 180.0 / M_PI;
}
