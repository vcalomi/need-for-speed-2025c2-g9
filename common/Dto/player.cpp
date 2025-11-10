#include "player.h"

#include "../common_codes.h"

PlayerDto::PlayerDto(const std::string& username, VehicleTipe Type):
        Dto(uint8_t(ActionCode::SEND_PLAYER)), username(username), Type(Type) {}
