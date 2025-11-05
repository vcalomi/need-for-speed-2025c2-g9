#include "player.h"

#include "../common_codes.h"

PlayerDto::PlayerDto(uint8_t id, VehicleTipe Type):
        Dto(uint8_t(ActionCode::SEND_PLAYER)), id(id), Type(Type) {}
