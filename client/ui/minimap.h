#pragma once
#include <SDL2pp/SDL2pp.hh>

#include "../core/camera.h"
#include "../core/map.h"
#include "../world/world.h"

class Minimap {
public:
    Minimap(SDL2pp::Renderer& renderer, const Map& map, int width, int height);
    void Render(const World& world, const Camera& camera);

private:
    SDL2pp::Renderer& renderer_;
    const SDL2pp::Texture& mapTexture_;
    int miniWidth, miniHeight;
};
