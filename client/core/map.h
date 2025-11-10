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
        SDL2pp::Rect dst(-camera.getX(), -camera.getY(), width_, height_);
        renderer.Copy(texture_, SDL2pp::NullOpt, dst);
    }


    int GetWidth() const { return width_; }
    int GetHeight() const { return height_; }

private:
    SDL2pp::Texture texture_;
    int width_;
    int height_;
};
