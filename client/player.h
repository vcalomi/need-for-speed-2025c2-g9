#pragma once
#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "./spritesheet.h"

class Player {
public:
    Player(const std::string& id, const std::string& carType, float x, float y);

    void HandleInput(const Uint8* keys, float delta);
    void Update(float delta);
    void Render(SDL2pp::Renderer& renderer, SpriteSheet& cars);

    std::string GetSpriteForAngle(float angle) const;

private:
    std::string id_;
    std::string carType_;
    std::string currentSprite_;
    float x_;
    float y_;
    float speed_;
    float angle_;
    float rotationSpeed_;
    float accel_;
    float turnDir_;
};
