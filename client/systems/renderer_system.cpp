#include "./renderer_system.h"

#include <iostream>

#include <SDL2/SDL_ttf.h>

#include "../events/checkpoint_event.h"

#define HALF_DIVISOR 2.0f

RendererSystem::RendererSystem(SDL2pp::Renderer& renderer, SpriteSheet& cars, World& world,
                               EventBus& eventBus):
        renderer_(renderer),
        cars_(cars),
        world_(world),
        eventBus_(eventBus),
        particleSystem_(renderer) {
    font_ = TTF_OpenFont("../client/lobby/assets/Tektur-SemiBold.ttf", 14);
    if (!font_) {
        std::cerr << "[RendererSystem] Failed to load font: " << TTF_GetError() << std::endl;
    } else {
        std::cout << "[RendererSystem] Font loaded successfully.\n";
    }

    eventBus_.Subscribe<PlayerStateUpdatedEvent>([this](const PlayerStateUpdatedEvent& e) {
        std::cout << "[RendererSystem] Received PlayerStateUpdatedEvent for player: " << e.username
                  << " isAccelerating=" << e.isAccelerating << " isBraking=" << e.isBraking
                  << std::endl;
        if (e.isBraking || e.isAccelerating) {
            SpawnParticlesFor(world_, e.username);
        }
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
    DrawCheckpoints(world, camera);
    for (const auto& [id, player]: world.GetPlayers()) {
        DrawPlayer(player, camera);
    }
    particleSystem_.Update(0.016f);
    particleSystem_.Render();
    minimap.Render(world, camera);
    renderer_.Present();
}

void RendererSystem::DrawPlayer(const Player& player, const Camera& camera) {
    std::string spriteName = player.GetSpriteForAngle(player.GetAngle());
    if (!cars_.HasSprite(spriteName)) {
        std::cerr << "[Renderer] Sprite '" << spriteName << "' not found, skipping draw\n";
        return;
    }

    const Rect& src = cars_.GetSprite(spriteName);

    float drawX = player.GetX() - camera.getX() - src.GetW() / HALF_DIVISOR;
    float drawY = player.GetY() - camera.getY() - src.GetH() / HALF_DIVISOR;

    Rect dest(drawX, drawY, src.GetW(), src.GetH());
    renderer_.Copy(cars_.GetTexture(), src, dest);
    DrawTextAbove(font_, player.GetUsername(), drawX, drawY, src);
}

void RendererSystem::DrawTextAbove(TTF_Font* font, const std::string& text, float drawX,
                                   float drawY, const Rect& src) {
    if (!font)
        return;

    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!textSurface)
        return;

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer_.Get(), textSurface);
    if (textTexture) {
        int textW, textH;
        SDL_QueryTexture(textTexture, nullptr, nullptr, &textW, &textH);

        SDL_Rect textRect = {static_cast<int>(drawX + src.GetW() / 2 - textW / 2),
                             static_cast<int>(drawY - textH - 5), textW, textH};

        SDL_RenderCopy(renderer_.Get(), textTexture, nullptr, &textRect);
        SDL_DestroyTexture(textTexture);
    }

    SDL_FreeSurface(textSurface);
}

void RendererSystem::SpawnParticlesFor(const World& world, const std::string& username) {
    std::cout << "[RendererSystem] Spawning particles for player: " << username << std::endl;
    const auto player = world.GetPlayer(username);
    particleSystem_.Emit(player.GetX(), player.GetY(), 8);
}

void RendererSystem::DrawCircle(SDL_Renderer* renderer, int x0, int y0, int radius) {
    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y) {
        SDL_RenderDrawPoint(renderer, x0 + x, y0 + y);
        SDL_RenderDrawPoint(renderer, x0 + y, y0 + x);
        SDL_RenderDrawPoint(renderer, x0 - y, y0 + x);
        SDL_RenderDrawPoint(renderer, x0 - x, y0 + y);
        SDL_RenderDrawPoint(renderer, x0 - x, y0 - y);
        SDL_RenderDrawPoint(renderer, x0 - y, y0 - x);
        SDL_RenderDrawPoint(renderer, x0 + y, y0 - x);
        SDL_RenderDrawPoint(renderer, x0 + x, y0 - y);

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

void RendererSystem::DrawCheckpoints(const World& world, const Camera& camera) {
    if (world.GetCheckpoints().empty()) {
        return;
    }

    SDL_SetRenderDrawColor(renderer_.Get(), 0, 255, 0, 255);

    for (const auto& checkpoint: world.GetCheckpoints()) {
        std::cout << checkpoint.x << " " << checkpoint.y << std::endl;
        float drawX = checkpoint.x - camera.getX();
        float drawY = checkpoint.y - camera.getY();

        int radius = 16;
        DrawCircle(renderer_.Get(), static_cast<int>(drawX), static_cast<int>(drawY), radius);
    }
}
