#include "renderer_system.h"

RendererSystem::RendererSystem(SDL2pp::Renderer& renderer, SpriteSheet& cars, World& world,
                               EventBus& bus):
        renderer_(renderer),
        carSprites_(cars),
        world_(world),
        text_(renderer, "../client/lobby/assets/Tektur-SemiBold.ttf", 14),
        background_(renderer),
        playerRenderer_(renderer, cars, nullptr),
        particleRenderer_(renderer),
        checkpointRenderer_(renderer),
        hudRenderer_(renderer, text_),
        screenRenderer_(renderer, text_),
        controller_(bus, particleRenderer_, world, state_),
        checkpointIndicator_(renderer) {
    checkpointIndicator_.SetTexture("../client/assets/need-for-speed/cars/arrow.png");
}


void RendererSystem::Render(const World& world, Map& map, const Camera& camera, Minimap& minimap) {
    renderer_.Clear();

    background_.RenderBackground(map, camera);

    if (state_.localPlayerExploded) {
        screenRenderer_.RenderExplosion(world, camera, 100);

        state_.explosionTimer -= 0.016f;
        if (state_.explosionTimer <= 0)
            state_.showExplosion = false;

        if (!state_.showExplosion)
            screenRenderer_.RenderPlayerLost();

        renderer_.Present();
        return;
    }

    if (state_.showPlayerFinishedScreen) {
        screenRenderer_.RenderPlayerFinished(state_.localFinishPosition, state_.localFinishTime);
        renderer_.Present();
        return;
    }

    if (state_.showFinalResultsScreen) {
        screenRenderer_.RenderRaceFinished(world);
        renderer_.Present();
        return;
    }

    checkpointRenderer_.Draw(world.GetCheckpoints(),
                             world.GetActiveCheckpointFor(world.GetLocalPlayer().GetUsername()),
                             world.GetPassedCheckpointIdsFor(world.GetLocalPlayer().GetUsername()),
                             camera);


    for (auto& [id, player]: world.GetPlayers()) {
        if (!player.IsAboveBridge()) {
            playerRenderer_.Draw(player, camera);
        }
    }

    particleRenderer_.Update(0.016f);
    particleRenderer_.Render(camera);


    background_.RenderForeground(map, camera);

    for (auto& [id, player]: world.GetPlayers()) {
        if (player.IsAboveBridge()) {
            playerRenderer_.Draw(player, camera);
        }
    }

    minimap.Render(world, camera, map);
    hudRenderer_.Render(world);

    const auto& local = world.GetLocalPlayer();
    const auto& nextCp = world.GetActiveCheckpointFor(local.GetUsername());
    checkpointIndicator_.Draw(camera, local, nextCp);

    renderer_.Present();
}
