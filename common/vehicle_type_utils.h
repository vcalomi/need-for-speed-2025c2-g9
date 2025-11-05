#pragma once
#include <string_view>
#include <stdexcept>

#include "./Dto/player.h"

static VehicleTipe toVehicleTipe(std::string_view key) {
    if (key == "fiat_600")    return VehicleTipe::FIAT_600;
    if (key == "ferrari_F40") return VehicleTipe::FERRARI_F40;
    if (key == "porsche_911") return VehicleTipe::PORSCHE_911;
    if (key == "sedan")       return VehicleTipe::SEDAN;
    if (key == "jeep")        return VehicleTipe::JEEP;
    if (key == "f100")        return VehicleTipe::F100;
    if (key == "truck")       return VehicleTipe::TRUCK;

    throw std::invalid_argument("Vehicle key unknown: " + std::string(key));
}
