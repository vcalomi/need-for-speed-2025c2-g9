#include "./player_renderer.h"


#define HALF_DIVISOR 2.0f

PlayerRenderer::PlayerRenderer(SDL2pp::Renderer& renderer, SpriteSheet& cars, TTF_Font* font):
        renderer_(renderer), cars_(cars), font_(font) {}

void PlayerRenderer::Draw(const Player& player, const Camera& camera) {
    // 1) Obtener sprite BASE del auto
    //    Debe llamarse algo como "Fiat_600_Base" o "Ferrari_F40_Base".
    std::string baseSpriteName = player.GetSprite();

    if (!cars_.HasSprite(baseSpriteName)) {
        std::cerr << "[Renderer] Base sprite '" << baseSpriteName << "' not found\n";
        return;
    }

    const Sprite& src = cars_.GetSprite(baseSpriteName);

    // 2) Convertir coordenadas de mundo → pantalla
    float drawX = player.GetX() - camera.getX();
    float drawY = player.GetY() - camera.getY();

    // 3) Crear rect de destino
    SDL_Rect dest;
    dest.w = src.width;
    dest.h = src.height;
    dest.x = static_cast<int>(drawX - dest.w / 2);
    dest.y = static_cast<int>(drawY - dest.h / 2);

    // 4) Rotación real en grados
    double angle = player.GetAngle();

    // 5) Renderizar usando SDL2 (dentro de SDL2pp)
    SDL_RenderCopyEx(renderer_.Get(),           // Renderer nativo
                     cars_.GetTexture().Get(),  // Textura nativa del spritesheet
                     &src.area,                 // SRC rect
                     &dest,                     // DEST rect
                     angle,                     // Ángulo real en grados
                     nullptr,                   // Rotar desde el centro
                     SDL_FLIP_NONE);

    // 6) Dibujar nombre encima (igual que siempre)
    DrawTextAbove(font_, player.GetUsername(), dest.x, dest.y, src.area);
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
