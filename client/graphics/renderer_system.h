#pragma once
#include <string>

#include <SDL2/SDL_ttf.h>
#include <SDL2pp/SDL2pp.hh>

#include "../core/camera.h"
#include "../core/map.h"
#include "../events/event.h"
#include "../events/player_events.h"
#include "../graphics/spritesheet.h"
#include "../ui/minimap.h"
#include "../world/world.h"

#include "particle_system.h"

class RendererSystem {
public:
    RendererSystem(SDL2pp::Renderer& renderer, SpriteSheet& cars, World& world, EventBus& eventBus);
    ~RendererSystem();

    void Render(const World& world, Map& map, const Camera& camera, Minimap& minimap);
    void DrawPlayer(const Player& player, const Camera& camera);
    void SpawnParticlesFor(const World& world, const std::string& username);

private:
    SDL2pp::Renderer& renderer_;
    SpriteSheet& cars_;
    World& world_;
    EventBus& eventBus_;
    TTF_Font* font_ = nullptr;
    ParticleSystem particleSystem_;

    void DrawTextAbove(TTF_Font* font, const std::string& text, float drawX, float drawY,
                       const Rect& src);
};
