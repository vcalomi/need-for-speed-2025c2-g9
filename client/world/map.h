#pragma once
#include <algorithm>
#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "camera.h"

class Map {
public:
    Map(SDL2pp::Renderer& renderer, const std::string& backgroundpath,
        const std::string& foregroundpath):
            backgroundTexture_(renderer, backgroundpath),
            foregroundTexture_(renderer, foregroundpath),
            width_(backgroundTexture_.GetWidth()),
            height_(backgroundTexture_.GetHeight()) {}

    void RenderBackground(SDL2pp::Renderer& renderer, const Camera& camera) {
        SDL2pp::Rect dst(-camera.getX(), -camera.getY(), width_, height_);
        renderer.Copy(backgroundTexture_, SDL2pp::NullOpt, dst);
    }

    void RenderForeground(SDL2pp::Renderer& renderer, const Camera& camera) {
        SDL2pp::Rect dst(-camera.getX(), -camera.getY(), width_, height_);
        renderer.Copy(foregroundTexture_, SDL2pp::NullOpt, dst);
    }

    int GetWidth() const { return width_; }
    int GetHeight() const { return height_; }
    const SDL2pp::Texture& GetTexture() const { return backgroundTexture_; }

private:
    SDL2pp::Texture backgroundTexture_;
    SDL2pp::Texture foregroundTexture_;
    int width_;
    int height_;
};
