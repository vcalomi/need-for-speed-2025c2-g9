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
