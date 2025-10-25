#pragma once
#include <SDL2pp/SDL2pp.hh>

#include "./spritesheet.h"
#include "./world.h"

class RendererSystem {
public:
    RendererSystem(SDL2pp::Renderer& renderer, SpriteSheet& cars);
    void Render(const World& world);

private:
    SDL2pp::Renderer& renderer_;
    SpriteSheet& cars_;

    void DrawPlayer(const Player& player);
};
