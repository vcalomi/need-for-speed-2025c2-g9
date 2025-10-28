#pragma once
#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "./spritesheet.h"

class Player {
public:
    Player(const std::string& id, const std::string& carType, float x, float y);

    void UpdateFromNetwork(float x, float y, float angle);

    float GetX() const { return x_; }
    float GetY() const { return y_; }
    float GetAngle() const { return angle_; }
    const std::string& GetId() const { return id_; }
    std::string GetSpriteForAngle(float angle) const;

private:
    std::string id_;
    std::string carType_;
    std::string currentSprite_;
    float x_;
    float y_;
    float angle_;
};
