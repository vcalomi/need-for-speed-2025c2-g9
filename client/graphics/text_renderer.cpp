#include "text_renderer.h"

#include <iostream>

TextRenderer::TextRenderer(SDL2pp::Renderer& renderer, const std::string& fontPath, int size):
        renderer_(renderer) {
    font_ = TTF_OpenFont(fontPath.c_str(), size);
    if (!font_) {
        std::cerr << "[TextRenderer] Failed to load font: " << TTF_GetError() << "\n";
    }
}

TextRenderer::~TextRenderer() {
    if (font_) {
        TTF_CloseFont(font_);
    }
}

void TextRenderer::Measure(const std::string& msg, int& w, int& h) {
    if (!font_) {
        w = h = 0;
        return;
    }

    if (TTF_SizeText(font_, msg.c_str(), &w, &h) != 0) {
        std::cerr << "[TextRenderer] Failed to measure text: " << TTF_GetError() << std::endl;
        w = h = 0;
    }
}


void TextRenderer::Draw(const std::string& text, int x, int y, SDL_Color color) {
    if (!font_)
        return;

    SDL_Surface* surf = TTF_RenderText_Blended(font_, text.c_str(), color);
    if (!surf)
        return;

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer_.Get(), surf);

    SDL_Rect dst{x, y, surf->w, surf->h};
    SDL_FreeSurface(surf);

    SDL_RenderCopy(renderer_.Get(), tex, nullptr, &dst);
    SDL_DestroyTexture(tex);
}

void TextRenderer::DrawPlayerName(const Player& player, const Camera& camera) {
    float wx = player.GetX();
    float wy = player.GetY();

    int sx = static_cast<int>(wx - camera.getX());
    int sy = static_cast<int>(wy - camera.getY());

    std::string name = player.GetUsername();

    int textW = 0, textH = 0;
    Measure(name, textW, textH);

    const int OFFSET_Y = -40;

    int drawX = sx - (textW / 2);
    int drawY = sy + OFFSET_Y;

    Draw(name, drawX, drawY, SDL_Color{255, 255, 255, 255});
}
