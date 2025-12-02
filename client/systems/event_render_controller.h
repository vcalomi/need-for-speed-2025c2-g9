#pragma once
#include "../events/event.h"
#include "../graphics/particle_renderer.h"
#include "../graphics/screen_renderer.h"
#include "../world/progress_manager.h"
#include "../world/world.h"

#include "player_final_results.h"

struct RenderState {
    bool raceFinished = false;
    bool localPlayerFinished = false;
    bool localPlayerExploded = false;
    int localFinishPosition = -1;
    float localFinishTime = 0.0f;
    float explosionTimer = 0.0f;
    bool showExplosion = false;
    bool showFinalResultsScreen = false;
    bool showFinalGameResultsScreen = false;
    bool showPlayerFinishedScreen = false;
    bool countdownActive = false;
    float countdownTimer = 0.0f;
    int countdownNumber = 3;
    float countdownGoTimer = 0.0f;
    std::vector<PlayerFinalResult> finalResults;
};

class EventRenderController {
public:
    EventRenderController(EventBus& bus, ParticleRenderer& particles, World& world,
                          RenderState& state, ScreenRenderer& screenRenderer,
                          ProgressManager& progress);

private:
    void RegisterEvents();

    EventBus& eventBus_;
    ParticleRenderer& particles_;
    World& world_;
    RenderState& state_;
    ScreenRenderer& screenRenderer_;
    ProgressManager& progress_;
};
