#ifndef PLAYER_MOVE_DTO_H
#define PLAYER_MOVE_DTO_H
#include <string>

#include "dto.h"

class PlayerMoveDto: public Dto {
public:
    std::string username;
    uint8_t move;
    PlayerMoveDto(const std::string& username, uint8_t move);
    PlayerMoveDto(const PlayerMoveDto&) = default;
};

#endif
