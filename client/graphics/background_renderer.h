#pragma once

#include <SDL2pp/Renderer.hh>

#include "../world/camera.h"
#include "../world/map.h"

class BackgroundRenderer {
public:
    explicit BackgroundRenderer(SDL2pp::Renderer& renderer);

    void RenderBackground(Map& map, const Camera& camera);
    void RenderForeground(Map& map, const Camera& camera);

private:
    SDL2pp::Renderer& renderer_;
};
