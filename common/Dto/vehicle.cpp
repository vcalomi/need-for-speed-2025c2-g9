#include "vehicle.h"

#include "../common_codes.h"

VehicleDto::VehicleDto(const std::string& username, float x, float y, float v, float speed,
                       float health, bool isAboveBridge):

        Dto(uint8_t(ActionCode::SEND_CARS)),
        username(username),
        x(x),
        y(y),
        rotation(v),
        speed(speed),
        health(health),
        isAboveBridge(isAboveBridge) {}
