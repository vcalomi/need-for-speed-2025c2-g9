#pragma once
#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "../graphics/spritesheet.h"

class Player {
public:
    Player(const int id, uint8_t carType, float x, float y);

    void UpdateFromNetwork(float x, float y, float angle);

    float GetX() const { return x_; }
    float GetY() const { return y_; }
    float GetAngle() const { return angle_; }
    int GetId() const { return id_; }
    std::string GetSpriteForAngle(float angle) const;

private:
    int id_;
    uint8_t carType_;
    std::string currentSprite_;
    float x_;
    float y_;
    float angle_;
};
