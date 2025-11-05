#ifndef PLAYER_DTO_H
#define PLAYER_DTO_H
#include <string>

#include "dto.h"

class PlayerDto: public Dto {
public:
    uint8_t id;
    uint8_t carType;
    PlayerDto(uint8_t id, uint8_t carType);
};

#endif