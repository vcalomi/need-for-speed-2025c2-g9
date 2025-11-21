#pragma once
#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "../../common/common_codes.h"
#include "../graphics/spritesheet.h"

class Player {
public:
    Player(std::string username, VehicleTipe carType, float x, float y, float health);

    void UpdateFromNetwork(float x, float y, float angle, float speed, bool isAboveBridge);

    float GetX() const { return x_; }
    float GetY() const { return y_; }
    float GetAngle() const { return angle_; }
    float GetHealth() const { return health_; }
    float GetSpeed() const { return speed_; }
    bool IsAboveBridge() const { return isAboveBridge_; }
    void setSpeed(float speed) { speed_ = speed; }
    void setIsAboveBridge(bool isAbove) { isAboveBridge_ = isAbove; }
    float GetMaxHealth() const { return maxHealth_; }
    std::string GetUsername() const { return username_; }
    std::string GetSprite() const;
    std::string VehicleTipeToString(VehicleTipe type) const;
    void updateHealth(float health) { health_ = health; }
    void setExploded(bool exploded) { exploded_ = exploded; }
    bool isExploded() const { return exploded_; }

private:
    std::string username_;
    VehicleTipe carType_;
    std::string baseSprite_;
    float x_;
    float y_;
    float angle_;
    float health_;
    const float maxHealth_;
    float speed_;
    bool isAboveBridge_;
    bool exploded_ = false;
};
