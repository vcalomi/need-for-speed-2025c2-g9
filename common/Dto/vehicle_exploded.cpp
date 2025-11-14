#include "vehicle_exploded.h"

#include "../common_codes.h"

VehicleExplodedDto::VehicleExplodedDto(const std::string& username):
        Dto(uint8_t(ActionCode::SEND_VEHICLE_EXPLODED)),
        username(username) {}
