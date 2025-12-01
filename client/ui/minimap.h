#pragma once
#include <SDL2pp/SDL2pp.hh>

#include "../world/camera.h"
#include "../world/map.h"
#include "../world/progress_manager.h"
#include "../world/world.h"

class Minimap {
public:
    Minimap(SDL2pp::Renderer& renderer, int width, int height);

    void Render(const World& world, const Camera& camera, const Map& map,
                ProgressManager& progress);

private:
    SDL2pp::Renderer& renderer_;
    SDL2pp::Texture minimapTexture_;
    int miniWidth;
    int miniHeight;
};
