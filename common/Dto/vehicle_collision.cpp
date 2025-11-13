#include "vehicle_collision.h"

#include "../common_codes.h"

VehicleCollisionDto::VehicleCollisionDto(const std::string& vehicle1_username, int vehicle_1_new_hp, 
                        const std::string& vehicle2_username, int vehicle_2_new_hp):
        Dto(uint8_t(ActionCode::SEND_VEHICLES_COLLISION)),
        vehicle1_username(vehicle1_username),
        vehicle_1_new_hp(vehicle_1_new_hp),
        vehicle2_username(vehicle2_username),
        vehicle_2_new_hp(vehicle_2_new_hp) {}
