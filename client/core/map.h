#pragma once
#include <algorithm>
#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "camera.h"

class Map {
public:
    Map(SDL2pp::Renderer& renderer, const std::string& path):
            texture_(renderer, path), width_(texture_.GetWidth()), height_(texture_.GetHeight()) {}

    void Render(SDL2pp::Renderer& renderer, const Camera& camera) {
        int camX = std::max(0, std::min(static_cast<int>(camera.x), width_ - camera.w));
        int camY = std::max(0, std::min(static_cast<int>(camera.y), height_ - camera.h));

        SDL2pp::Rect src(camX, camY, camera.w, camera.h);
        SDL2pp::Rect dest(0, 0, camera.w, camera.h);
        renderer.Copy(texture_, src, dest);
    }

    int GetWidth() const { return width_; }
    int GetHeight() const { return height_; }

private:
    SDL2pp::Texture texture_;
    int width_;
    int height_;
};
