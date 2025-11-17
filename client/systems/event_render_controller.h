#pragma once
#include "../events/event.h"
#include "../graphics/particle_renderer.h"
#include "../world/world.h"

struct RenderState {
    bool raceFinished = false;
    bool localPlayerExploded = false;
    float explosionTimer = 0.0f;
    bool showExplosion = false;
};

class EventRenderController {
public:
    EventRenderController(EventBus& bus, ParticleRenderer& particles, const World& world,
                          RenderState& state);

private:
    void RegisterEvents();

    EventBus& eventBus_;
    ParticleRenderer& particles_;
    const World& world_;
    RenderState& state_;
};
