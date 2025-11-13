#include "player.h"

#include "../common_codes.h"

PlayerDto::PlayerDto(const std::string& username, VehicleTipe Type, int  car_hp):
        Dto(uint8_t(ActionCode::SEND_PLAYER)), username(username), Type(Type), car_hp(car_hp) {}
