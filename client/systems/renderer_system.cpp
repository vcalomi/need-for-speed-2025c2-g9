#include "./renderer_system.h"

#include <iostream>

#include <SDL2/SDL_ttf.h>

#include "../events/checkpoint_completed_event.h"
#include "../events/checkpoint_event.h"
#include "../events/player_collision_event.h"
#include "../events/wall_collision_event.h"
#include "../ui/checkpoint_indicator.h"

RendererSystem::RendererSystem(SDL2pp::Renderer& renderer, SpriteSheet& cars, World& world,
                               EventBus& eventBus):
        renderer_(renderer),
        cars_(cars),
        world_(world),
        eventBus_(eventBus),
        particleRenderer_(renderer),
        checkpointRenderer_(renderer),
        playerRenderer_(renderer, cars, nullptr) {

    font_ = TTF_OpenFont("../client/lobby/assets/Tektur-SemiBold.ttf", 14);
    if (!font_) {
        std::cerr << "[RendererSystem] Failed to load font: " << TTF_GetError() << std::endl;
    } else {
        std::cout << "[RendererSystem] Font loaded successfully.\n";
        playerRenderer_.SetFont(font_);
    }

    eventBus_.Subscribe<PlayerMoveEvent>([this](const PlayerMoveEvent& e) {
        auto move_mask = static_cast<uint8_t>(e.move);
        if (move_mask == static_cast<uint8_t>(MoveMask::BRAKE))
            SpawnParticlesFor(world_, e.username, ParticleType::SMOKE_BRAKE);
        if (move_mask == static_cast<uint8_t>(MoveMask::ACCELERATE))
            SpawnParticlesFor(world_, e.username, ParticleType::SMOKE_ACCEL);
    });

    eventBus_.Subscribe<WallCollisionEvent>([this](const WallCollisionEvent& e) {
        SpawnParticlesFor(world_, e.player1_username, ParticleType::SPARK_WALL);
    });

    eventBus_.Subscribe<PlayerCollisionEvent>([this](const PlayerCollisionEvent& e) {
        SpawnParticlesFor(world_, e.player1_username, ParticleType::SPARK_VEHICLE);
        SpawnParticlesFor(world_, e.player2_username, ParticleType::SPARK_VEHICLE);
    });
}


RendererSystem::~RendererSystem() {
    if (font_) {
        TTF_CloseFont(font_);
        font_ = nullptr;
    }
}

void RendererSystem::Render(const World& world, Map& map, const Camera& camera, Minimap& minimap) {
    renderer_.Clear();
    CheckpointIndicator checkpointIndicator(renderer_);
    map.RenderBackground(renderer_, camera);
    if (!world.HasPlayers()) {
        renderer_.Present();
        return;
    }
    const auto& localPlayer = world.GetLocalPlayer();
    const auto& activeCp = world.GetActiveCheckpointFor(localPlayer.GetUsername());
    const auto passed = world.GetPassedCheckpointIdsFor(localPlayer.GetUsername());
    checkpointRenderer_.Draw(world.GetCheckpoints(), activeCp, passed, camera);
    for (const auto& [id, player]: world.GetPlayers()) {
        playerRenderer_.Draw(player, camera);
    }
    particleRenderer_.Update(0.016f);
    particleRenderer_.Render(camera);
    map.RenderForeground(renderer_, camera);
    minimap.Render(world, camera);
    checkpointIndicator.Draw(camera, localPlayer, activeCp);
    renderer_.Present();
}

void RendererSystem::SpawnParticlesFor(const World& world, const std::string& username,
                                       ParticleType type) {
    const auto player = world.GetPlayer(username);

    const Sprite& sprite = player.GetCurrentSprite(cars_);
    float angle = player.GetAngle();

    float offset = 0.0f;

    switch (type) {
        case ParticleType::SMOKE_ACCEL:
            offset = sprite.backOffset;
            break;

        case ParticleType::SMOKE_BRAKE:
            offset = sprite.frontOffset;
            break;

        case ParticleType::SPARK_WALL:
        case ParticleType::SPARK_VEHICLE:
            offset = 0.0f;
            break;
    }

    particleRenderer_.Emit(player.GetX(), player.GetY(), angle, offset, type, 8);
}
