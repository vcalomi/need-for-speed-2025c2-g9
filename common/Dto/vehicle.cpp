#include "vehicle.h"

#include "../common_codes.h"

VehicleDto::VehicleDto(uint8_t id, float x, float y, float v):
        Dto(uint8_t(ActionCode::SEND_CARS)), id(id), x(x), y(y), rotation(v) {}
