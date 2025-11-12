#include "./renderer_system.h"

#include <iostream>

#include <SDL2/SDL_ttf.h>

#include "../events/checkpoint_event.h"

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

    eventBus_.Subscribe<PlayerStateUpdatedEvent>([this](const PlayerStateUpdatedEvent& e) {
        if (e.isBraking || e.isAccelerating)
            SpawnParticlesFor(world_, e.username);
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
    map.Render(renderer_, camera);
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
    particleRenderer_.Render();
    minimap.Render(world, camera);
    renderer_.Present();
}

void RendererSystem::SpawnParticlesFor(const World& world, const std::string& username) {
    std::cout << "[RendererSystem] Spawning particles for player: " << username << std::endl;
    const auto player = world.GetPlayer(username);
    particleRenderer_.Emit(player.GetX(), player.GetY(), 8);
}
