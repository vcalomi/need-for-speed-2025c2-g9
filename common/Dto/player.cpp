#include "player.h"

#include "../common_codes.h"

PlayerDto::PlayerDto(uint8_t id, uint8_t carType):
        Dto(uint8_t(ActionCode::SEND_PLAYER)), id(id), carType(carType) {}
