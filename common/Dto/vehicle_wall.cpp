#include "vehicle_wall.h"

#include "../common_codes.h"

VehicleWallCollisionDto::VehicleWallCollisionDto(int vehicle_id):
        Dto(uint8_t(ActionCode::SEND_VEHICLE_WALL_COLLISION)), vehicle_id(vehicle_id) {}
