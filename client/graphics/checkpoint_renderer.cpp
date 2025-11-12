#include "./checkpoint_renderer.h"

#include <cmath>

#include <SDL2/SDL.h>

CheckpointRenderer::CheckpointRenderer(SDL2pp::Renderer& renderer): renderer_(renderer) {}

void CheckpointRenderer::DrawCircle(int x0, int y0, int radius) {
    int dx = 1;
    int dy = 1;
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

void CheckpointRenderer::DrawAnimated(int x, int y, int baseRadius) {
    Uint32 ticks = SDL_GetTicks();

    float pulse = std::sin(ticks * 0.005f) * 2.0f;
    int radius = baseRadius + static_cast<int>(pulse);
    Uint8 intensity = 150 + static_cast<Uint8>(105 * (std::sin(ticks * 0.005f) * 0.5f + 0.5f));

    renderer_.SetDrawBlendMode(SDL_BLENDMODE_BLEND);

    renderer_.SetDrawColor(intensity, intensity, intensity, 60);
    DrawCircle(x, y, radius + 4);

    renderer_.SetDrawColor(255, 255, 255, 255);
    DrawCircle(x, y, radius);

    renderer_.SetDrawColor(intensity, intensity, intensity, 200);
    SDL_Rect center = {x - 2, y - 2, 4, 4};
    SDL_RenderFillRect(renderer_.Get(), &center);
}

void CheckpointRenderer::Draw(const std::vector<Checkpoint>& checkpoints, const Camera& camera) {
    if (checkpoints.empty())
        return;

    for (const auto& cp: checkpoints) {
        float drawX = cp.x - camera.getX();
        float drawY = cp.y - camera.getY();
        DrawAnimated(static_cast<int>(drawX), static_cast<int>(drawY), 16);
    }
}
