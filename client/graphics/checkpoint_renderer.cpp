#include "./checkpoint_renderer.h"

#include <cmath>

#include <SDL2/SDL.h>

namespace {

// ---- DrawCircle constants ----
constexpr int INITIAL_DX = 1;
constexpr int INITIAL_DY = 1;

// ---- Animation constants ----
constexpr float PULSE_SPEED = 0.005f;
constexpr float PULSE_AMPLITUDE = 2.0f;

// ---- Radii ----
constexpr int PASSED_RADIUS = 28;
constexpr int ACTIVE_RADIUS = 32;
constexpr int INACTIVE_RADIUS = 28;

// ---- Halo ----
constexpr int HALO_EXTRA_RADIUS = 4;
constexpr float HALO_ALPHA_FACTOR = 1.0f / 3.0f;

// ---- Center rectangle ----
constexpr int CENTER_HALF_SIZE = 2;
constexpr int CENTER_SIZE = CENTER_HALF_SIZE * 2;

// ---- Bright center alpha increase ----
constexpr Uint8 CENTER_EXTRA_ALPHA = 50;

}  // namespace

CheckpointRenderer::CheckpointRenderer(SDL2pp::Renderer& renderer): renderer_(renderer) {}

void CheckpointRenderer::DrawCircle(int x0, int y0, int radius) {
    int dx = INITIAL_DX;
    int dy = INITIAL_DY;
    int err = dx - (radius << 1);
    int x = radius - 1;
    int y = 0;

    while (x >= y) {
        renderer_.DrawPoint(x0 + x, y0 + y);
        renderer_.DrawPoint(x0 + y, y0 + x);
        renderer_.DrawPoint(x0 - y, y0 + x);
        renderer_.DrawPoint(x0 - x, y0 + y);
        renderer_.DrawPoint(x0 - x, y0 - y);
        renderer_.DrawPoint(x0 - y, y0 - x);
        renderer_.DrawPoint(x0 + y, y0 - x);
        renderer_.DrawPoint(x0 + x, y0 - y);

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

void CheckpointRenderer::DrawAnimated(int x, int y, int baseRadius, SDL_Color color) {
    Uint32 ticks = SDL_GetTicks();

    float pulse = std::sin(ticks * PULSE_SPEED) * PULSE_AMPLITUDE;
    int radius = baseRadius + static_cast<int>(pulse);
    Uint8 alpha = color.a;

    renderer_.SetDrawBlendMode(SDL_BLENDMODE_BLEND);

    renderer_.SetDrawColor(color.r, color.g, color.b,
                           static_cast<Uint8>(alpha * HALO_ALPHA_FACTOR));
    DrawCircle(x, y, radius + HALO_EXTRA_RADIUS);

    renderer_.SetDrawColor(color.r, color.g, color.b, alpha);
    DrawCircle(x, y, radius);

    SDL_SetRenderDrawColor(renderer_.Get(), color.r, color.g, color.b,
                           std::min<Uint8>(255, alpha + CENTER_EXTRA_ALPHA));
    SDL_Rect center = {x - CENTER_HALF_SIZE, y - CENTER_HALF_SIZE, CENTER_SIZE, CENTER_SIZE};
    SDL_RenderFillRect(renderer_.Get(), &center);
}

void CheckpointRenderer::Draw(const std::vector<Checkpoint>& checkpoints,
                              const Checkpoint& activeCheckpoint,
                              const std::set<int>& passedCheckpointIds, const Camera& camera) {

    for (const auto& cp: checkpoints) {
        float drawX = cp.x - camera.getX();
        float drawY = cp.y - camera.getY();

        if (passedCheckpointIds.count(cp.id)) {
            SDL_Color passed = {0, 255, 0, 100};
            DrawAnimated(drawX, drawY, PASSED_RADIUS, passed);

        } else if (cp.id == activeCheckpoint.id) {
            SDL_Color active = {255, 255, 255, 255};
            DrawAnimated(drawX, drawY, ACTIVE_RADIUS, active);

        } else {
            SDL_Color inactive = {150, 150, 150, 120};
            DrawAnimated(drawX, drawY, INACTIVE_RADIUS, inactive);
        }
    }
}
