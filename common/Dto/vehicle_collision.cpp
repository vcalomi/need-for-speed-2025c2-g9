#include "vehicle_collision.h"

#include "../common_codes.h"

VehicleCollisionDto::VehicleCollisionDto(const std::string& vehicle1_username, const std::string& vehicle2_username):
        Dto(uint8_t(ActionCode::SEND_VEHICLES_COLLISION)),
        vehicle1_username(vehicle1_username),
        vehicle2_username(vehicle2_username) {}
