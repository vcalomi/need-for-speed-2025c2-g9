#pragma once
#include "../events/event.h"
#include "../graphics/particle_renderer.h"
#include "../graphics/screen_renderer.h"
#include "../world/world.h"

struct RenderState {
    bool raceFinished = false;
    bool localPlayerFinished = false;
    bool localPlayerExploded = false;
    int localFinishPosition = -1;
    float localFinishTime = 0.0f;
    float explosionTimer = 0.0f;
    bool showExplosion = false;
    bool showFinalResultsScreen = false;
    bool showPlayerFinishedScreen = false;
};

class EventRenderController {
public:
    EventRenderController(EventBus& bus, ParticleRenderer& particles, World& world,
                          RenderState& state, ScreenRenderer& screenRenderer);

private:
    void RegisterEvents();

    EventBus& eventBus_;
    ParticleRenderer& particles_;
    World& world_;
    RenderState& state_;
    ScreenRenderer& screenRenderer_;
};
