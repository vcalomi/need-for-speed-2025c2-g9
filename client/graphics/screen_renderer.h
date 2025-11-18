#pragma once

#include <memory>
#include <string>
#include <vector>

#include <SDL2pp/Renderer.hh>

#include "../world/camera.h"
#include "../world/world.h"

#include "text_renderer.h"

struct ExplosionAnim {
    float worldX, worldY;
    float size;

    float timePerFrame = 0.03f;
    float elapsed = 0.0f;
    int frame = 0;

    bool finished = false;
};


class ScreenRenderer {
public:
    ScreenRenderer(SDL2pp::Renderer& renderer, TextRenderer& text);

    void RenderExplosion(const Camera& camera, float size);
    void RenderPlayerLost();
    void RenderRaceFinished(const World& world);
    void RenderPlayerFinished(int position, float timeSeconds);
    void TriggerExplosion(float worldX, float worldY, float size);

private:
    SDL2pp::Renderer& renderer_;
    TextRenderer& text_;

    std::vector<ExplosionAnim> explosions_;
    std::unique_ptr<SDL2pp::Texture> explosionTexture_;


    void DrawCentered(const std::string& msg, int y);
};
