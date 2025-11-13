#include "vehicle_wall_collision.h"

#include "../common_codes.h"

VehicleWallCollisionDto::VehicleWallCollisionDto(const std::string& username, float vehicle_new_hp):
        Dto(uint8_t(ActionCode::SEND_VEHICLE_WALL_COLLISION)), 
        username(username),
        vehicle_new_hp(vehicle_new_hp) {}
