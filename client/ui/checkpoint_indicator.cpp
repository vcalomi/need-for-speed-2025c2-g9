#include "./checkpoint_indicator.h"

void CheckpointIndicator::Draw(const Camera& camera, const Player& localPlayer,
                               const Checkpoint& activeCp) {
    auto [dx, dy] = ComputeDirection(localPlayer, activeCp);
    if (dx == 0 && dy == 0)
        return;

    SDL_FPoint start = ComputeArrowStart(localPlayer, camera, dx, dy);
    auto [left, right, tip] = ComputeTrianglePoints(start, dx, dy);

    renderer_.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    renderer_.SetDrawColor(255, 255, 255, 220);

    DrawFilledTriangle(left, right, tip);
}

std::pair<float, float> CheckpointIndicator::ComputeDirection(const Player& player,
                                                              const Checkpoint& checkpoint) const {
    float dx = checkpoint.x - player.GetX();
    float dy = checkpoint.y - player.GetY();
    float len = std::sqrt(dx * dx + dy * dy);
    if (len < 0.001f)
        return {0, 0};
    return {dx / len, dy / len};
}

SDL_FPoint CheckpointIndicator::ComputeArrowStart(const Player& player, const Camera& camera,
                                                  float dx, float dy) const {
    float baseX = player.GetX() - camera.getX();
    float baseY = player.GetY() - camera.getY();
    const float offsetForward = 40.0f;
    return {baseX + dx * offsetForward, baseY + dy * offsetForward};
}

std::tuple<SDL_FPoint, SDL_FPoint, SDL_FPoint> CheckpointIndicator::ComputeTrianglePoints(
        const SDL_FPoint& start, float dx, float dy) const {
    const float arrowLength = 35.0f;
    const float arrowWidth = 20.0f;

    SDL_FPoint tip = {start.x + dx * arrowLength, start.y + dy * arrowLength};
    SDL_FPoint left = {start.x - dy * (arrowWidth / 2.0f), start.y + dx * (arrowWidth / 2.0f)};
    SDL_FPoint right = {start.x + dy * (arrowWidth / 2.0f), start.y - dx * (arrowWidth / 2.0f)};
    return {left, right, tip};
}

void CheckpointIndicator::DrawFilledTriangle(SDL_FPoint a, SDL_FPoint b, SDL_FPoint c) {
    if (b.y < a.y)
        std::swap(a, b);
    if (c.y < a.y)
        std::swap(a, c);
    if (c.y < b.y)
        std::swap(b, c);

    for (int y = static_cast<int>(a.y); y <= static_cast<int>(c.y); ++y) {
        bool secondHalf = y > b.y || b.y == a.y;
        float segmentHeight = secondHalf ? c.y - b.y : b.y - a.y;
        if (segmentHeight == 0)
            continue;

        float alpha = (y - a.y) / (c.y - a.y);
        float beta = (y - (secondHalf ? b.y : a.y)) / segmentHeight;

        SDL_FPoint A = {a.x + (c.x - a.x) * alpha, a.y + (c.y - a.y) * alpha};
        SDL_FPoint B = secondHalf ? SDL_FPoint{b.x + (c.x - b.x) * beta, b.y + (c.y - b.y) * beta} :
                                    SDL_FPoint{a.x + (b.x - a.x) * beta, a.y + (b.y - a.y) * beta};

        if (A.x > B.x)
            std::swap(A, B);
        renderer_.DrawLine((int)A.x, y, (int)B.x, y);
    }
}
