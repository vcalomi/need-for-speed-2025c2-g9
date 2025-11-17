#pragma once

#include <SDL2pp/Renderer.hh>

#include "../world/world.h"

#include "text_renderer.h"

class HUDRenderer {
public:
    HUDRenderer(SDL2pp::Renderer& renderer, TextRenderer& text);

    void Render(const World& world);

private:
    SDL2pp::Renderer& renderer_;
    TextRenderer& text_;

    void RenderLapCounter(const World& world);
    void RenderHealthBar(const World& world);
};
