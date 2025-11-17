#include "player_move.h"

#include <utility>

#include "../common_codes.h"

PlayerMoveDto::PlayerMoveDto(const std::string& username, uint8_t move):
        Dto(uint8_t(ActionCode::SEND_PLAYER_MOVE), username),
        username(username),
        move(move) {}
