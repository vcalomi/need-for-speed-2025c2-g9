#pragma once
#include <string>

#include <SDL2pp/SDL2pp.hh>

class Map {
public:
    Map(SDL2pp::Renderer& renderer, const std::string& path):
            texture_(renderer, path), width_(texture_.GetWidth()), height_(texture_.GetHeight()) {}

    void Render(SDL2pp::Renderer& renderer) {
        SDL2pp::Rect src(0, 0, width_, height_);
        SDL2pp::Rect dest(0, 0, width_, height_);
        renderer.Copy(texture_, src, dest);
    }

    int GetWidth() const { return width_; }
    int GetHeight() const { return height_; }

private:
    SDL2pp::Texture texture_;
    int width_;
    int height_;
};
