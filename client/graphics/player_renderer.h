#pragma once
#include <string>

#include <SDL2/SDL_ttf.h>
#include <SDL2pp/SDL2pp.hh>

#include "../graphics/spritesheet.h"
#include "../world/camera.h"
#include "../world/player.h"

class PlayerRenderer {
public:
    PlayerRenderer(SDL2pp::Renderer& renderer, SpriteSheet& cars, TTF_Font* font);

    void Draw(const Player& player, const Camera& camera);
    void SetFont(TTF_Font* font);

private:
    SDL2pp::Renderer& renderer_;
    SpriteSheet& cars_;
    TTF_Font* font_ = nullptr;

    void DrawTextAbove(const std::string& text, int screenX, int screenY, const Sprite& sprite);
};
