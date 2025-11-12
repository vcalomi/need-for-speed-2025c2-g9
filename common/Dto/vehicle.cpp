#include "vehicle.h"

#include "../common_codes.h"

VehicleDto::VehicleDto(const std::string& username, float x, float y, float v, bool isAccelerating,
                       bool isBraking, bool isAboveBridge):
        Dto(uint8_t(ActionCode::SEND_CARS)),
        username(username),
        x(x),
        y(y),
        rotation(v),
        isAccelerating(isAccelerating),
        isBraking(isBraking),
        isAboveBridge(isAboveBridge) {}
