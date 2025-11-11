#include "./player.h"

#include <cmath>

using SDL2pp::Renderer;

Player::Player(std::string username, VehicleTipe carType, float x, float y):
        username_(username), carType_(carType), x_(x), y_(y), angle_(0.0f) {
    currentSprite_ = VehicleTipeToString(carType) + "Norte";
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

void Player::UpdateFromNetwork(float x, float y, float angleRad) {
    x_ = x;
    y_ = y;

    float angleDeg = (angleRad * 180.0f / static_cast<float>(M_PI)) + 90.0f;

    while (angleDeg < 0) angleDeg += 360;
    while (angleDeg >= 360) angleDeg -= 360;

    angle_ = angleDeg;
    currentSprite_ = GetSpriteForAngle(angleDeg);
}


std::string Player::GetSpriteForAngle(float angleDeg) const {
    auto vehicleType = VehicleTipeToString(carType_);
    while (angleDeg < 0) angleDeg += 360;
    while (angleDeg >= 360) angleDeg -= 360;

    if (angleDeg >= 345 || angleDeg < 15)
        return vehicleType + "Este";
    if (angleDeg < 35)
        return vehicleType + "Sureste20";
    if (angleDeg < 60)
        return vehicleType + "Sureste45";
    if (angleDeg < 80)
        return vehicleType + "Sureste75";
    if (angleDeg < 110)
        return vehicleType + "Sur";
    if (angleDeg < 125)
        return vehicleType + "Suroeste75";
    if (angleDeg < 150)
        return vehicleType + "Suroeste45";
    if (angleDeg < 170)
        return vehicleType + "Suroeste20";
    if (angleDeg < 200)
        return vehicleType + "Oeste";
    if (angleDeg < 215)
        return vehicleType + "Noroeste20";
    if (angleDeg < 240)
        return vehicleType + "Noroeste45";
    if (angleDeg < 260)
        return vehicleType + "Noroeste75";
    if (angleDeg < 285)
        return vehicleType + "Norte";
    if (angleDeg < 310)
        return vehicleType + "Noreste75";
    if (angleDeg < 330)
        return vehicleType + "Noreste45";
    if (angleDeg < 345)
        return vehicleType + "Noreste20";
    return vehicleType + "Sur";
}
