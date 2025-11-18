#pragma once
#include <string>

#include <SDL2/SDL_ttf.h>
#include <SDL2pp/Renderer.hh>

class TextRenderer {
public:
    TextRenderer(SDL2pp::Renderer& renderer, const std::string& fontPath, int size);
    ~TextRenderer();
    void Measure(const std::string& msg, int& w, int& h);

    void Draw(const std::string& text, int x, int y, SDL_Color color = {255, 255, 255, 255});

private:
    SDL2pp::Renderer& renderer_;
    TTF_Font* font_ = nullptr;
};
