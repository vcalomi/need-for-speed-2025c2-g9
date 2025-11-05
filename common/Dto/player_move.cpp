#include "player_move.h"

#include "../common_codes.h"

PlayerMoveDto::PlayerMoveDto(uint8_t id, uint8_t move):
        Dto(uint8_t(ActionCode::SEND_PLAYER_MOVE)), id(id), move(move) {}
