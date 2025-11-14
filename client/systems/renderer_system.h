#pragma once
#include <string>

#include <SDL2/SDL_ttf.h>
#include <SDL2pp/SDL2pp.hh>

#include "../events/event.h"
#include "../events/player_events.h"
#include "../graphics/checkpoint_renderer.h"
#include "../graphics/particle_renderer.h"
#include "../graphics/player_renderer.h"
#include "../graphics/spritesheet.h"
#include "../ui/minimap.h"
#include "../world/camera.h"
#include "../world/map.h"
#include "../world/world.h"

class RendererSystem {
public:
    RendererSystem(SDL2pp::Renderer& renderer, SpriteSheet& cars, World& world, EventBus& eventBus);
    ~RendererSystem();

    void Render(const World& world, Map& map, const Camera& camera, Minimap& minimap);
    void SpawnParticlesFor(const World& world, const std::string& username, ParticleType type);

private:
    SDL2pp::Renderer& renderer_;
    SpriteSheet& cars_;
    World& world_;
    EventBus& eventBus_;
    TTF_Font* font_ = nullptr;
    ParticleRenderer particleRenderer_;
    PlayerRenderer playerRenderer_;
    CheckpointRenderer checkpointRenderer_;

    void RenderLapCounter(const World& world);
    void DrawText(const std::string& text, int x, int y);
    void RenderHealthBar(const World& world);
};
