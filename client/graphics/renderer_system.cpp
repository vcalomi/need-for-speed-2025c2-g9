#include "./renderer_system.h"

#include <iostream>

#include <SDL2/SDL_ttf.h>

#define HALF_DIVISOR 2.0f

RendererSystem::RendererSystem(SDL2pp::Renderer& renderer, SpriteSheet& cars):
        renderer_(renderer), cars_(cars), particleSystem_(renderer) {
    font_ = TTF_OpenFont("../client/lobby/assets/Tektur-SemiBold.ttf", 14);
    if (!font_) {
        std::cerr << "[RendererSystem] Failed to load font: " << TTF_GetError() << std::endl;
    } else {
        std::cout << "[RendererSystem] Font loaded successfully.\n";
    }
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
    for (const auto& [id, player]: world.GetPlayers()) {
        DrawPlayer(player, camera);
    }
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
