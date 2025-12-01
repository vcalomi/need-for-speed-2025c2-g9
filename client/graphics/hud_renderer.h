#pragma once

#include <SDL2pp/Renderer.hh>

#include "../world/progress_manager.h"
#include "../world/world.h"

#include "text_renderer.h"

class HUDRenderer {
public:
    HUDRenderer(SDL2pp::Renderer& renderer, TextRenderer& text);

    void Render(const World& world, ProgressManager& progress);

private:
    SDL2pp::Renderer& renderer_;
    TextRenderer& text_;

    void RenderLapCounter(const World& world, ProgressManager& progress);
    void RenderHealthBar(const World& world);
};
