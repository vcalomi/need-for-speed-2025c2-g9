#pragma once
#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "../../common/common_codes.h"
#include "../graphics/spritesheet.h"

class Player {
public:
    Player(std::string username, VehicleTipe carType, float x, float y);

    void UpdateFromNetwork(float x, float y, float angle);

    float GetX() const { return x_; }
    float GetY() const { return y_; }
    float GetAngle() const { return angle_; }
    std::string GetUsername() const { return username_; }
    std::string GetSpriteForAngle(float angle) const;
    std::string VehicleTipeToString(VehicleTipe type) const;
    const Sprite& GetCurrentSprite(const SpriteSheet& carSprites) const;

private:
    std::string username_;
    VehicleTipe carType_;
    std::string currentSprite_;
    float x_;
    float y_;
    float angle_;
};
