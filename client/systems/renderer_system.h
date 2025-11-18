#pragma once

#include <SDL2pp/Renderer.hh>

#include "../graphics/background_renderer.h"
#include "../graphics/checkpoint_renderer.h"
#include "../graphics/hud_renderer.h"
#include "../graphics/particle_renderer.h"
#include "../graphics/player_renderer.h"
#include "../graphics/screen_renderer.h"
#include "../graphics/text_renderer.h"
#include "../ui/checkpoint_indicator.h"
#include "../ui/minimap.h"
#include "../ui/speedometer.h"
#include "../world/camera.h"
#include "../world/map.h"
#include "../world/world.h"
#include "./event_render_controller.h"

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
    Speedometer speedometer_;

    RenderState state_;
    EventRenderController controller_;
};
