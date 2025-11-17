#include "event_render_controller.h"

#include "../events/player_collision_event.h"
#include "../events/player_events.h"
#include "../events/race_finished_event.h"
#include "../events/vehicle_exploded_event.h"
#include "../events/wall_collision_event.h"

EventRenderController::EventRenderController(EventBus& bus, ParticleRenderer& particles,
                                             const World& world, RenderState& state):
        eventBus_(bus), particles_(particles), world_(world), state_(state) {
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

    eventBus_.Subscribe<RaceFinishedEvent>(
            [this](const RaceFinishedEvent&) { state_.raceFinished = true; });

    eventBus_.Subscribe<VehicleExplodedEvent>([this](const VehicleExplodedEvent& e) {
        if (e.username == world_.GetLocalPlayer().GetUsername()) {
            state_.localPlayerExploded = true;
            state_.showExplosion = true;
            state_.explosionTimer = 1.0f;
        }
    });
}
