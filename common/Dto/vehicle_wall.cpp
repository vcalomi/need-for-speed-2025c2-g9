#include "vehicle_wall.h"

#include "../common_codes.h"

VehicleWallDto::VehicleWallDto(int vehicle_id):
        Dto(uint8_t(ActionCode::SEND_VEHICLE_WALL)), vehicle_id(vehicle_id) {}
