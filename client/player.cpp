#include "./player.h"

#include <cmath>

using SDL2pp::Renderer;

Player::Player(const std::string& id, const std::string& carType, float x, float y):
        id_(id),
        carType_(carType),
        currentSprite_(carType + "Norte"),
        x_(x),
        y_(y),
        speed_(150.0f),
        angle_(0.0f),
        rotationSpeed_(180.0f),
        accel_(0.0f),
        turnDir_(0.0f) {}

void Player::ApplyInput(float accelInput, float turnInput, float delta) {
    accel_ = accelInput;
    turnDir_ = turnInput;
    angle_ += turnDir_ * rotationSpeed_ * delta;

    if (angle_ < 0)
        angle_ += 360;
    if (angle_ >= 360)
        angle_ -= 360;
}

void Player::UpdateFromNetwork(float x, float y, float angle, float speed) {
    x_ = x;
    y_ = y;
    angle_ = angle;
    speed_ = speed;
    currentSprite_ = GetSpriteForAngle(angle_);
}

void Player::Update(float delta) {
    float radians = angle_ * M_PI / 180.0f;
    x_ += cos(radians) * speed_ * accel_ * delta;
    y_ += sin(radians) * speed_ * accel_ * delta;
    currentSprite_ = GetSpriteForAngle(angle_);
}

std::string Player::GetSpriteForAngle(float angleDeg) const {
    while (angleDeg < 0) angleDeg += 360;
    while (angleDeg >= 360) angleDeg -= 360;

    if (angleDeg >= 345 || angleDeg < 15)
        return carType_ + "Este";
    if (angleDeg < 35)
        return carType_ + "Sureste20";
    if (angleDeg < 60)
        return carType_ + "Sureste45";
    if (angleDeg < 80)
        return carType_ + "Sureste75";
    if (angleDeg < 110)
        return carType_ + "Sur";
    if (angleDeg < 125)
        return carType_ + "Suroeste75";
    if (angleDeg < 150)
        return carType_ + "Suroeste45";
    if (angleDeg < 170)
        return carType_ + "Suroeste20";
    if (angleDeg < 200)
        return carType_ + "Oeste";
    if (angleDeg < 215)
        return carType_ + "Noroeste20";
    if (angleDeg < 240)
        return carType_ + "Noroeste45";
    if (angleDeg < 260)
        return carType_ + "Noroeste75";
    if (angleDeg < 285)
        return carType_ + "Norte";
    if (angleDeg < 310)
        return carType_ + "Noreste75";
    if (angleDeg < 330)
        return carType_ + "Noreste45";
    if (angleDeg < 345)
        return carType_ + "Noreste20";
    return carType_ + "Sur";
}
