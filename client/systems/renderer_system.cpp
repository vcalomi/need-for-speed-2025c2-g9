#include "renderer_system.h"

#include <string>

#include "../events/countdown_down_event.h"
#include "../events/countdown_go_event.h"
#include "../events/new_npc_event.h"
#include "../events/npc_hit_event.h"
#include "../events/upgrade_car_event.h"

RendererSystem::RendererSystem(SDL2pp::Renderer& renderer, SpriteSheet& cars, SpriteSheet& npcs,
                               World& world, EventBus& bus, ProgressManager& progress):
        renderer_(renderer),
        carSprites_(cars),
        npcSprites_(npcs),
        world_(world),
        progress_(progress),
        text_(renderer, "../client/lobby/assets/Tektur-SemiBold.ttf", 14),
        background_(renderer),
        playerRenderer_(renderer, cars, nullptr),
        particleRenderer_(renderer),
        checkpointRenderer_(renderer),
        hudRenderer_(renderer, text_),
        screenRenderer_(renderer, text_),
        controller_(bus, particleRenderer_, world, state_, screenRenderer_, progress),
        checkpointIndicator_(renderer),
        speedometer_(renderer, "../client/assets/need-for-speed/cars/speedometer.png",
                     "../client/assets/need-for-speed/cars/speedometer_needle.png"),
        eventBus_(bus) {
    checkpointIndicator_.SetTexture("../client/assets/need-for-speed/cars/arrow.png");

    eventBus_.Subscribe<NewNpcEvent>([this](const NewNpcEvent& e) {
        int spriteId = (e.id % 12);
        if (spriteId == 0)
            spriteId = 12;

        std::string spriteName = "npc_" + std::to_string(spriteId);

        world_.AddNpc(e.id, e.x, e.y, spriteName);
    });

    eventBus_.Subscribe<NpcHitEvent>([this](const NpcHitEvent& e) {
        auto player = world_.GetPlayer(e.username);
        particleRenderer_.Emit(player.GetX(), player.GetY(), 0.0f, 0.0f, ParticleType::BLOOD, 10);
        world_.RemoveNpcById(e.npcId);
    });
}


void RendererSystem::Render(const World& world, Map& map, const Camera& camera, Minimap& minimap) {
    renderer_.Clear();

    if (state_.showFinalGameResultsScreen) {
        screenRenderer_.RenderGameFinalResults(state_.finalResults);
        renderer_.Present();
        return;
    }

    if (state_.countdownActive) {
        float dt = 0.016f;

        state_.countdownTimer -= dt;

        int newNum = std::ceil(state_.countdownTimer);

        if (newNum != state_.countdownNumber && newNum > 0) {
            state_.countdownNumber = newNum;
            eventBus_.Publish(CountdownDownEvent());
        }

        if (state_.countdownTimer <= 0.0f) {
            state_.countdownActive = false;
            state_.countdownNumber = 0;
            state_.countdownGoTimer = 1.0f;
            eventBus_.Publish(CountdownGoEvent());
        }
    } else if (state_.countdownNumber == 0) {

        float dt = 0.016f;

        state_.countdownGoTimer -= dt;

        if (state_.countdownGoTimer <= 0.0f) {
            state_.countdownNumber = -1;
        }
    }


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
        screenRenderer_.RenderRaceFinished(state_.localFinishPosition, state_.localFinishTime);
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
            text_.DrawPlayerName(player, camera);
        }
    }

    for (const auto& [id, npc]: world.GetNpcs()) {
        auto sprite = npcSprites_.GetSprite(npc.spriteName);
        auto rect = sprite.area;
        auto tex = sprite.texture;

        int sx = static_cast<int>(npc.x - camera.getX());
        int sy = static_cast<int>(npc.y - camera.getY());

        renderer_.Copy(*tex, rect, SDL2pp::Rect(sx, sy, sprite.width, sprite.height));
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
            text_.DrawPlayerName(player, camera);
        }
    }

    minimap.Render(world, camera, map, progress_);
    hudRenderer_.Render(world, progress_);

    const auto& local = world.GetLocalPlayer();
    const auto& nextCp = progress_.GetActiveCheckpoint();
    checkpointIndicator_.Draw(camera, local, nextCp);

    float speed = world.GetLocalPlayer().GetSpeed();
    speedometer_.Render(speed, 200.0f);

    if (state_.countdownActive || state_.countdownNumber >= 0) {
        screenRenderer_.RenderCountdown(state_.countdownTimer, state_.countdownNumber);
    }

    renderer_.Present();
}
