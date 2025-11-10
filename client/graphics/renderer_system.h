#pragma once
#include <string>

#include <SDL2/SDL_ttf.h>
#include <SDL2pp/SDL2pp.hh>

#include "../core/camera.h"
#include "../core/map.h"
#include "../graphics/spritesheet.h"
#include "../ui/minimap.h"
#include "../world/world.h"

class RendererSystem {
public:
    RendererSystem(SDL2pp::Renderer& renderer, SpriteSheet& cars);
    ~RendererSystem();

    void Render(const World& world, Map& map, const Camera& camera, Minimap& minimap);
    void DrawPlayer(const Player& player, const Camera& camera);

private:
    SDL2pp::Renderer& renderer_;
    SpriteSheet& cars_;
    TTF_Font* font_ = nullptr;

    void DrawTextAbove(TTF_Font* font, const std::string& text, float drawX, float drawY,
                       const Rect& src);
};
