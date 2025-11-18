#pragma once

#include <string>

#include <SDL2pp/Renderer.hh>

#include "../world/camera.h"
#include "../world/world.h"

#include "text_renderer.h"

class ScreenRenderer {
public:
    ScreenRenderer(SDL2pp::Renderer& renderer, TextRenderer& text);

    void RenderExplosion(const World& world, const Camera& camera, float size);
    void RenderPlayerLost();
    void RenderRaceFinished(const World& world);
    void RenderPlayerFinished(int position, float timeSeconds);

private:
    SDL2pp::Renderer& renderer_;
    TextRenderer& text_;

    void DrawCentered(const std::string& msg, int y);
};
