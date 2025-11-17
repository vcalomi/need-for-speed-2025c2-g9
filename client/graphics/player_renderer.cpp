#include "player_renderer.h"

#include <iostream>

PlayerRenderer::PlayerRenderer(SDL2pp::Renderer& renderer, SpriteSheet& cars, TTF_Font* font):
        renderer_(renderer), cars_(cars), font_(font) {}


void PlayerRenderer::Draw(const Player& player, const Camera& camera) {
    std::string spriteName = player.GetSprite();
    if (!spriteName.empty())
        spriteName[0] = std::tolower(spriteName[0]);


    if (!cars_.HasSprite(spriteName)) {
        std::cerr << "[PlayerRenderer] Sprite not found: " << spriteName << "\n";
        return;
    }

    const Sprite& src = cars_.GetSprite(spriteName);

    // Convertir coordenadas world -> screen
    float worldX = player.GetX();
    float worldY = player.GetY();

    int screenX = static_cast<int>(worldX - camera.getX());
    int screenY = static_cast<int>(worldY - camera.getY());

    // Rect destino
    SDL_Rect dest;
    dest.w = src.width;
    dest.h = src.height;
    dest.x = screenX - dest.w / 2;
    dest.y = screenY - dest.h / 2;

    // Rotación real
    double angle = player.GetAngle() + 180.0;

    // AHORA USAMOS LA TEXTURA DEL SPRITE
    SDL_RenderCopyEx(renderer_.Get(),
                     src.texture->Get(),  // textura particular del auto
                     &src.area,           // el rect dentro de la textura
                     &dest, angle, nullptr, SDL_FLIP_NONE);

    // Dibujar nombre arriba del auto
    DrawTextAbove(player.GetUsername(), dest.x, dest.y, src);
}


void PlayerRenderer::SetFont(TTF_Font* font) { font_ = font; }

void PlayerRenderer::DrawTextAbove(const std::string& text, int carX, int carY,
                                   const Sprite& sprite) {
    if (!font_)
        return;

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* rawSurface = TTF_RenderText_Blended(font_, text.c_str(), white);
    if (!rawSurface)
        return;

    SDL2pp::Surface surf(rawSurface);
    SDL2pp::Texture texture(renderer_, surf);

    int textW = surf.Get()->w;
    int textH = surf.Get()->h;

    SDL_Rect rect;
    rect.w = textW;
    rect.h = textH;
    rect.x = carX + (sprite.width / 2) - (textW / 2);
    rect.y = carY - textH - 4;

    renderer_.Copy(texture, SDL2pp::NullOpt, rect);
}
