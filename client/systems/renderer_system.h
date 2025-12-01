#pragma once

#include <SDL2pp/Renderer.hh>

#include "../events/event.h"
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
#include "../world/progress_manager.h"
#include "../world/world.h"
#include "./event_render_controller.h"

class RendererSystem {
public:
    RendererSystem(SDL2pp::Renderer& renderer, SpriteSheet& carSprites, World& world, EventBus& bus,
                   ProgressManager& progress);
    void Render(const World& world, Map& map, const Camera& camera, Minimap& minimap);

private:
    SDL2pp::Renderer& renderer_;
    SpriteSheet& carSprites_;
    World& world_;
    ProgressManager& progress_;
    TextRenderer text_;
    BackgroundRenderer background_;
    PlayerRenderer playerRenderer_;
    ParticleRenderer particleRenderer_;
    CheckpointRenderer checkpointRenderer_;
    HUDRenderer hudRenderer_;
    ScreenRenderer screenRenderer_;
    CheckpointIndicator checkpointIndicator_;
    Speedometer speedometer_;
    EventBus& eventBus_;
    RenderState state_;
    EventRenderController controller_;
};
