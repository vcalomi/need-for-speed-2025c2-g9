#ifndef PLAYER_MOVE_DTO_H
#define PLAYER_MOVE_DTO_H
#include <string>

#include "dto.h"

class PlayerMoveDto: public Dto {
public:
    uint8_t id;
    uint8_t move;
    PlayerMoveDto(uint8_t id, uint8_t move);
};

#endif