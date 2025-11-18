#pragma once

#include <string>

#include <SDL2pp/SDL2pp.hh>

class Speedometer {
public:
    Speedometer(SDL2pp::Renderer& renderer, const std::string& dialPath,
                const std::string& needlePath);
    void Render(float speed, float maxSpeed);

private:
    SDL2pp::Renderer& renderer_;
    SDL2pp::Texture dial_;
    SDL2pp::Texture needle_;

    float angleMin_ = -130.0f;
    float angleMax_ = 130.0f;
};
