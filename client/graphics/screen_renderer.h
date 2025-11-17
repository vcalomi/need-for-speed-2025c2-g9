#pragma once

#include <SDL2pp/Renderer.hh>

#include "../world/world.h"

#include "text_renderer.h"

class ScreenRenderer {
public:
    ScreenRenderer(SDL2pp::Renderer& renderer, TextRenderer& text);

    void RenderExplosion(const World& world, float size);
    void RenderPlayerLost();
    void RenderRaceFinished(const World& world);

private:
    SDL2pp::Renderer& renderer_;
    TextRenderer& text_;
};
