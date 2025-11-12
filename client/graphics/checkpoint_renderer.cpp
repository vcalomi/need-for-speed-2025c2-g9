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

void CheckpointRenderer::DrawAnimated(int x, int y, int baseRadius, SDL_Color color) {
    Uint32 ticks = SDL_GetTicks();

    float pulse = std::sin(ticks * 0.005f) * 2.0f;
    int radius = baseRadius + static_cast<int>(pulse);
    Uint8 alpha = color.a;

    renderer_.SetDrawBlendMode(SDL_BLENDMODE_BLEND);

    // Halo exterior semitransparente
    renderer_.SetDrawColor(color.r, color.g, color.b, alpha / 3);
    DrawCircle(x, y, radius + 4);

    // Contorno principal
    renderer_.SetDrawColor(color.r, color.g, color.b, alpha);
    DrawCircle(x, y, radius);

    // Centro brillante
    SDL_SetRenderDrawColor(renderer_.Get(), color.r, color.g, color.b, std::min(255, alpha + 50));
    SDL_Rect center = {x - 2, y - 2, 4, 4};
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
            DrawAnimated(drawX, drawY, 14, passed);
        } else if (cp.id == activeCheckpoint.id) {
            SDL_Color active = {255, 255, 255, 255};
            DrawAnimated(drawX, drawY, 18, active);
        } else {
            SDL_Color inactive = {150, 150, 150, 120};
            DrawAnimated(drawX, drawY, 14, inactive);
        }
    }
}
