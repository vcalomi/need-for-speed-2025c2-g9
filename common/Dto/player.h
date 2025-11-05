#ifndef PLAYER_DTO_H
#define PLAYER_DTO_H
#include <string>
#include "../common_codes.h"
#include "dto.h"

class PlayerDto: public Dto {
public:
    uint8_t id;
    VehicleTipe Type;
    PlayerDto(uint8_t id, VehicleTipe Type);
};

#endif