#include "./player_renderer.h"


#define HALF_DIVISOR 2.0f

PlayerRenderer::PlayerRenderer(SDL2pp::Renderer& renderer, SpriteSheet& cars, TTF_Font* font):
        renderer_(renderer), cars_(cars), font_(font) {}

void PlayerRenderer::Draw(const Player& player, const Camera& camera) {
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

void PlayerRenderer::SetFont(TTF_Font* font) { font_ = font; }

void PlayerRenderer::DrawTextAbove(TTF_Font* font, const std::string& text, float drawX,
                                   float drawY, const Rect& src) {
    if (!font)
        return;

    SDL_Color color = {255, 255, 255, 255};

    SDL2pp::Surface textSurface(TTF_RenderText_Blended(font, text.c_str(), color));
    if (textSurface.Get() == nullptr)
        return;

    SDL2pp::Texture textTexture(renderer_, textSurface);

    int textW = textSurface.Get()->w;
    int textH = textSurface.Get()->h;

    SDL_Rect textRect = {static_cast<int>(drawX + src.GetW() / 2 - textW / 2),
                         static_cast<int>(drawY - textH - 5), textW, textH};

    renderer_.Copy(textTexture, SDL2pp::NullOpt, textRect);
}
