#include "player_move.h"

#include <utility>

#include "../common_codes.h"

PlayerMoveDto::PlayerMoveDto(uint8_t id, uint8_t move):
        Dto(static_cast<uint8_t>(ActionCode::SEND_PLAYER_MOVE), id), id(id), move(move) {}
