#include "vehicle_wall_collision.h"

#include "../common_codes.h"

VehicleWallCollisionDto::VehicleWallCollisionDto(const std::string& username):
        Dto(uint8_t(ActionCode::SEND_VEHICLE_WALL_COLLISION)), username(username) {}
