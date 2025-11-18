#include "./player.h"

#include <cmath>

using SDL2pp::Renderer;

Player::Player(std::string username, VehicleTipe carType, float x, float y, float health):
        username_(username),
        carType_(carType),
        x_(x),
        y_(y),
        angle_(0.0f),
        health_(health),
        maxHealth_(health) {
    baseSprite_ = VehicleTipeToString(carType) + "_Base";
}

std::string Player::VehicleTipeToString(VehicleTipe type) const {
    switch (type) {
        case VehicleTipe::FIAT_600:
            return "Fiat_600";
        case VehicleTipe::FERRARI_F40:
            return "Ferrari_F40";
        case VehicleTipe::PORSCHE_911:
            return "Porsche_911";
        case VehicleTipe::SEDAN:
            return "Sedan";
        case VehicleTipe::JEEP:
            return "Jeep";
        case VehicleTipe::F100:
            return "F100";
        case VehicleTipe::TRUCK:
            return "Truck";
        default:
            return "Unknown";
    }
}

void Player::UpdateFromNetwork(float x, float y, float angleRad, float speed, bool isAboveBridge) {
    x_ = x;
    y_ = y;

    float angleDeg = angleRad * 180.0f / M_PI;

    // Normalizar
    while (angleDeg < 0) angleDeg += 360;
    while (angleDeg >= 360) angleDeg -= 360;

    angle_ = angleDeg;
    speed_ = speed;
    isAboveBridge_ = isAboveBridge;
}


std::string Player::GetSprite() const { return baseSprite_; }
