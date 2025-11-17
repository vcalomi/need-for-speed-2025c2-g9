#pragma once

#include <SDL2pp/Renderer.hh>

#include "../ui/checkpoint_indicator.h"
#include "../ui/minimap.h"
#include "../world/camera.h"
#include "../world/map.h"
#include "../world/world.h"

#include "background_renderer.h"
#include "checkpoint_renderer.h"
#include "event_render_controller.h"
#include "hud_renderer.h"
#include "particle_renderer.h"
#include "player_renderer.h"
#include "screen_renderer.h"
#include "text_renderer.h"

class RendererSystem {
public:
    RendererSystem(SDL2pp::Renderer& renderer, SpriteSheet& carSprites, World& world,
                   EventBus& bus);
    void Render(const World& world, Map& map, const Camera& camera, Minimap& minimap);

private:
    SDL2pp::Renderer& renderer_;
    SpriteSheet& carSprites_;
    World& world_;

    TextRenderer text_;
    BackgroundRenderer background_;
    PlayerRenderer playerRenderer_;
    ParticleRenderer particleRenderer_;
    CheckpointRenderer checkpointRenderer_;
    HUDRenderer hudRenderer_;
    ScreenRenderer screenRenderer_;
    CheckpointIndicator checkpointIndicator_;

    RenderState state_;
    EventRenderController controller_;
};
