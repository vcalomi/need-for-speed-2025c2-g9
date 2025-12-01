#include "event_render_controller.h"

#include "../events/player_collision_event.h"
#include "../events/player_events.h"
#include "../events/player_race_finished_event.h"
#include "../events/race_finished_event.h"
#include "../events/race_info_event.h"
#include "../events/vehicle_exploded_event.h"
#include "../events/wall_collision_event.h"

EventRenderController::EventRenderController(EventBus& bus, ParticleRenderer& particles,
                                             World& world, RenderState& state,
                                             ScreenRenderer& screenRenderer):
        eventBus_(bus),
        particles_(particles),
        world_(world),
        state_(state),
        screenRenderer_(screenRenderer) {
    RegisterEvents();
}

void EventRenderController::RegisterEvents() {

    eventBus_.Subscribe<PlayerMoveEvent>([this](const PlayerMoveEvent& e) {
        auto mask = static_cast<uint8_t>(e.move);

        if (mask == static_cast<uint8_t>(MoveMask::BRAKE))
            particles_.EmitForPlayer(world_, e.username, ParticleType::SMOKE_BRAKE);

        if (mask == static_cast<uint8_t>(MoveMask::ACCELERATE))
            particles_.EmitForPlayer(world_, e.username, ParticleType::SMOKE_ACCEL);
    });

    eventBus_.Subscribe<WallCollisionEvent>([this](const WallCollisionEvent& e) {
        particles_.EmitForPlayer(world_, e.player1_username, ParticleType::SPARK_WALL);
    });

    eventBus_.Subscribe<PlayerCollisionEvent>([this](const PlayerCollisionEvent& e) {
        particles_.EmitForPlayer(world_, e.player1_username, ParticleType::SPARK_VEHICLE);
        particles_.EmitForPlayer(world_, e.player2_username, ParticleType::SPARK_VEHICLE);
    });

    eventBus_.Subscribe<RaceInfoEvent>([this](const RaceInfoEvent&) {
        if (state_.raceFinished) {
            state_.raceFinished = false;
            state_.localPlayerFinished = false;
            state_.localPlayerExploded = false;
            state_.localFinishPosition = -1;
            state_.localFinishTime = 0.0f;
            state_.explosionTimer = 0.0f;
            state_.showExplosion = false;
            state_.showFinalResultsScreen = false;
            state_.showPlayerFinishedScreen = false;
            return;
        }
    });


    eventBus_.Subscribe<RaceFinishedEvent>([this](const RaceFinishedEvent&) {
        state_.raceFinished = true;
        state_.showPlayerFinishedScreen = false;
        state_.showFinalResultsScreen = true;
        world_.ResetPlayersExploded();
    });


    eventBus_.Subscribe<PlayerRaceFinishedEvent>([this](const PlayerRaceFinishedEvent& e) {
        if (e.username == world_.GetLocalPlayer().GetUsername()) {
            state_.localPlayerFinished = true;
            state_.localFinishPosition = e.finalPosition;
            state_.localFinishTime = e.finishTime;
            state_.showPlayerFinishedScreen = true;
        }
    });


    eventBus_.Subscribe<VehicleExplodedEvent>([this](const VehicleExplodedEvent& e) {
        if (e.username == world_.GetLocalPlayer().GetUsername()) {
            state_.localPlayerExploded = true;
            state_.showExplosion = true;
            state_.explosionTimer = 1.0f;

            auto& p = world_.GetLocalPlayer();
            screenRenderer_.TriggerExplosion(p.GetX(), p.GetY(), 240.0f);
        } else {
            auto& p = world_.GetPlayer(e.username);
            screenRenderer_.TriggerExplosion(p.GetX(), p.GetY(), 240.0f);
            p.setExploded(true);
        }
    });
}
