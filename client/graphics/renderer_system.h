#pragma once
#include <SDL2pp/SDL2pp.hh>

#include "../core/map.h"
#include "../ui/minimap.h"
#include "../world/world.h"
#include "./spritesheet.h"

class RendererSystem {
public:
    RendererSystem(SDL2pp::Renderer& renderer, SpriteSheet& cars);
    void Render(const World& world, Map& map, const Camera& camera, Minimap& minimap);

private:
    SDL2pp::Renderer& renderer_;
    SpriteSheet& cars_;

    void DrawPlayer(const Player& player, const Camera& camera);
};
