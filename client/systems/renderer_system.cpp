#include "renderer_system.h"

#include "../events/upgrade_car_event.h"

RendererSystem::RendererSystem(SDL2pp::Renderer& renderer, SpriteSheet& cars, World& world,
                               EventBus& bus, ProgressManager& progress):
        renderer_(renderer),
        carSprites_(cars),
        world_(world),
        eventBus_(bus),
        progress_(progress),
        text_(renderer, LOBBY_ASSETS_DIR "/Tektur-SemiBold.ttf", 14),
        background_(renderer),
        playerRenderer_(renderer, cars, nullptr),
        particleRenderer_(renderer),
        checkpointRenderer_(renderer),
        hudRenderer_(renderer, text_),
        screenRenderer_(renderer, text_),
        controller_(bus, particleRenderer_, world, state_, screenRenderer_, progress),
        checkpointIndicator_(renderer),
        speedometer_(renderer, ASSETS_DIR "/cars/speedometer.png",
                     ASSETS_DIR "/cars/speedometer_needle.png") {
    checkpointIndicator_.SetTexture(ASSETS_DIR "/cars/arrow.png");
}


void RendererSystem::Render(const World& world, Map& map, const Camera& camera, Minimap& minimap) {
    renderer_.Clear();

    background_.RenderBackground(map, camera);

    if (state_.localPlayerExploded) {
        float dt = 0.016f;

        screenRenderer_.RenderExplosion(camera, dt);

        state_.explosionTimer -= dt;
        if (state_.explosionTimer <= 0) {
            screenRenderer_.RenderPlayerLost();
        }

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

        int mx, my;
        Uint32 mouseState = SDL_GetMouseState(&mx, &my);
        bool pressed = mouseState & SDL_BUTTON(SDL_BUTTON_LEFT);

        UpgradeChoice choice = screenRenderer_.HandleUpgradeInput(mx, my, pressed);

        if (choice == UpgradeChoice::CONFIRM) {
            bool h = screenRenderer_.IsHealthSelected();
            bool s = screenRenderer_.IsSpeedSelected();

            eventBus_.Publish(UpgradeCarEvent(world_.GetLocalPlayer().GetUsername(), h, s));

            screenRenderer_.LockUpgrades();
        }

        SDL_Delay(16);
        return;
    }


    checkpointRenderer_.Draw(world.GetCheckpoints(), progress_.GetActiveCheckpoint(),
                             progress_.GetPassedCheckpoints(), camera);


    for (auto& [id, player]: world.GetPlayers()) {
        if (player.isExploded()) {
            continue;
        }
        if (!player.IsAboveBridge()) {
            playerRenderer_.Draw(player, camera);
        }
    }

    particleRenderer_.Update(0.016f);
    particleRenderer_.Render(camera);


    background_.RenderForeground(map, camera);

    for (auto& [id, player]: world.GetPlayers()) {
        if (player.isExploded()) {
            continue;
        }
        if (player.IsAboveBridge()) {
            playerRenderer_.Draw(player, camera);
        }
    }

    minimap.Render(world, camera, map, progress_);
    hudRenderer_.Render(world, progress_);

    const auto& local = world.GetLocalPlayer();
    const auto& nextCp = progress_.GetActiveCheckpoint();
    checkpointIndicator_.Draw(camera, local, nextCp);

    float speed = world.GetLocalPlayer().GetSpeed();
    speedometer_.Render(speed, 200.0f);

    renderer_.Present();
}
