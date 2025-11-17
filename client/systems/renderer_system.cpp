#include "./renderer_system.h"

#include <iomanip>
#include <iostream>

#include <SDL2/SDL_ttf.h>

#include "../events/checkpoint_completed_event.h"
#include "../events/checkpoint_event.h"
#include "../events/player_collision_event.h"
#include "../events/race_finished_event.h"
#include "../events/vehicle_exploded_event.h"
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

    eventBus_.Subscribe<RaceFinishedEvent>(
            [this](const RaceFinishedEvent& e) { raceFinished_ = true; });

    eventBus_.Subscribe<VehicleExplodedEvent>([this](const VehicleExplodedEvent& e) {
        if (e.username == world_.GetLocalPlayer().GetUsername()) {
            localPlayerExploded_ = true;
            showExplosion_ = true;
            explosionTimer_ = 1.0f;
        }
    });
}


RendererSystem::~RendererSystem() {
    if (font_) {
        TTF_CloseFont(font_);
        font_ = nullptr;
    }
}

void RendererSystem::RenderWorldWithoutLocal(const World& world, Map& map, const Camera& camera,
                                             Minimap& minimap) {
    CheckpointIndicator checkpointIndicator(renderer_);
    map.RenderBackground(camera);

    const auto& local = world.GetLocalPlayer();
    const std::string localName = local.GetUsername();

    for (const auto& [id, player]: world.GetPlayers()) {
        if (player.GetUsername() == localName)
            continue;

        playerRenderer_.Draw(player, camera);
    }

    particleRenderer_.Update(0.016f);
    particleRenderer_.Render(camera);

    const auto& activeCp = world.GetActiveCheckpointFor(localName);
    const auto passed = world.GetPassedCheckpointIdsFor(localName);

    checkpointRenderer_.Draw(world.GetCheckpoints(), activeCp, passed, camera);
    map.RenderForeground(camera);
    minimap.Render(world, camera);
}

void RendererSystem::RenderExplosion(const World& world) {
    const auto& local = world.GetLocalPlayer();
    float x = local.GetX();
    float y = local.GetY();

    SDL_SetRenderDrawColor(renderer_.Get(), 255, 200, 50, 200);
    SDL_FRect boom{x - 50, y - 50, 100, 100};
    SDL_RenderFillRectF(renderer_.Get(), &boom);
}

void RendererSystem::RenderPlayerLostScreen(const World& world) {
    int w, h;
    SDL_GetRendererOutputSize(renderer_.Get(), &w, &h);

    SDL_SetRenderDrawColor(renderer_.Get(), 0, 0, 0, 180);
    SDL_Rect bg{0, 0, w, h};
    SDL_RenderFillRect(renderer_.Get(), &bg);

    DrawText("YOU EXPLODED!", w / 2 - 100, 150);
    DrawText("You will appear in the next race.", w / 2 - 150, 220);
    DrawText("Waiting for next race...", w / 2 - 120, h - 100);
}


void RendererSystem::Render(const World& world, Map& map, const Camera& camera, Minimap& minimap) {
    renderer_.Clear();

    if (localPlayerExploded_) {
        renderer_.Clear();

        RenderWorldWithoutLocal(world, map, camera, minimap);


        if (showExplosion_) {
            RenderExplosion(world);
            explosionTimer_ -= 0.016f;
            if (explosionTimer_ <= 0.0f) {
                showExplosion_ = false;
            }
        } else {
            RenderPlayerLostScreen(world);
        }

        renderer_.Present();
        return;
    }


    if (raceFinished_) {
        RenderRaceFinishedScreen(world);
        renderer_.Present();
        return;
    }

    CheckpointIndicator checkpointIndicator(renderer_);
    map.RenderBackground(camera);
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
    map.RenderForeground(camera);
    minimap.Render(world, camera);
    checkpointIndicator.Draw(camera, localPlayer, activeCp);
    RenderLapCounter(world);
    RenderHealthBar(world);
    renderer_.Present();
}

void RendererSystem::SpawnParticlesFor(const World& world, const std::string& username,
                                       ParticleType type) {
    const auto player = world.GetPlayer(username);

    const Sprite& sprite = cars_.GetSprite(player.GetSprite());
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


void RendererSystem::DrawText(const std::string& text, int x, int y) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font_, text.c_str(), white);
    if (!surface)
        return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_.Get(), surface);

    SDL_Rect dst{x, y, surface->w, surface->h};
    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer_.Get(), texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
}

void RendererSystem::RenderLapCounter(const World& world) {
    const auto& localPlayer = world.GetLocalPlayer();

    int laps = world.GetLapsFor(localPlayer.GetUsername());
    int totalLaps = 3;

    int currentCp = world.GetLapProgressFor(localPlayer.GetUsername());
    int totalCp = world.GetCheckpoints().size();

    std::string lapText = "Lap " + std::to_string(laps + 1) + "/" + std::to_string(totalLaps);
    std::string cpText = "CP " + std::to_string(currentCp) + "/" + std::to_string(totalCp);


    int width = 0, height = 0;
    SDL_GetRendererOutputSize(renderer_.Get(), &width, &height);

    int margin = 20;


    int lineHeight = 22;

    int x = margin;
    int yLap = height - margin - lineHeight * 2;
    int yCp = height - margin - lineHeight;

    DrawText(lapText, x, yLap);
    DrawText(cpText, x, yCp);
}

void RendererSystem::RenderHealthBar(const World& world) {
    const Player& local = world.GetLocalPlayer();

    float health = local.GetHealth();
    float maxHealth = local.GetMaxHealth();

    float ratio = (maxHealth > 0) ? (health / maxHealth) : 0.0f;
    ratio = std::clamp(ratio, 0.0f, 1.0f);

    int sw = 0, sh = 0;
    SDL_GetRendererOutputSize(renderer_.Get(), &sw, &sh);

    int margin = 20;
    int barWidth = 200;
    int barHeight = 18;

    int x = margin;
    int y = sh - margin - barHeight - 90;

    SDL_Rect bgRect{x, y, barWidth, barHeight};
    SDL_SetRenderDrawColor(renderer_.Get(), 40, 40, 40, 200);
    SDL_RenderFillRect(renderer_.Get(), &bgRect);

    int hpWidth = static_cast<int>(barWidth * ratio);

    Uint8 r = 0, g = 255, b = 0;
    if (ratio < 0.3f) {
        r = 255;
        g = 40;
        b = 40;
    } else if (ratio < 0.6f) {
        r = 255;
        g = 165;
        b = 0;
    }

    SDL_Rect hpRect{x, y, hpWidth, barHeight};
    SDL_SetRenderDrawColor(renderer_.Get(), r, g, b, 255);
    SDL_RenderFillRect(renderer_.Get(), &hpRect);

    std::stringstream ss;
    ss << std::fixed << std::setprecision((health == floor(health)) ? 0 : 1);
    ss << health << "/" << maxHealth;
    std::string hpText = ss.str();

    SDL_Color white{255, 255, 255, 255};
    SDL_Surface* surf = TTF_RenderText_Blended(font_, hpText.c_str(), white);
    if (!surf)
        return;

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer_.Get(), surf);
    int tw = surf->w;
    int th = surf->h;
    SDL_FreeSurface(surf);

    SDL_Rect dst{x + (barWidth - tw) / 2, y + (barHeight - th) / 2, tw, th};
    SDL_RenderCopy(renderer_.Get(), tex, nullptr, &dst);
    SDL_DestroyTexture(tex);
}

void RendererSystem::RenderRaceFinishedScreen(const World& world) {
    int w, h;
    SDL_GetRendererOutputSize(renderer_.Get(), &w, &h);

    // Fondo oscuro
    SDL_SetRenderDrawColor(renderer_.Get(), 0, 0, 0, 200);
    SDL_Rect bg{0, 0, w, h};
    SDL_RenderFillRect(renderer_.Get(), &bg);

    // Título
    DrawText("RACE FINISHED", w / 2 - 100, 80);

    // Mostrar datos del jugador local
    const auto& local = world.GetLocalPlayer();
    // std::string time = "Your time: " + std::to_string(local.GetRaceTime());
    // DrawText(time, w / 2 - 100, 150);

    // Si guardaste standings en lastRaceInfo_
    // DrawText("Position: " + std::to_string(lastRaceInfo_.position), w / 2 - 100, 190);

    // Mensaje de espera
    DrawText("Waiting for next race...", w / 2 - 120, h - 100);
}
