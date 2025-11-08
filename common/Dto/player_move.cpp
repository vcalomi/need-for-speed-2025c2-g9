#include "player_move.h"

#include <utility>

#include "../common_codes.h"

PlayerMoveDto::PlayerMoveDto(std::string username, uint8_t move):
        Dto(static_cast<uint8_t>(ActionCode::SEND_PLAYER_MOVE), username),
        username(username),
        move(move) {}
