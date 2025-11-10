#ifndef PLAYER_DTO_H
#define PLAYER_DTO_H
#include <string>

#include "../common_codes.h"

#include "dto.h"


class PlayerDto: public Dto {
public:
    std::string username;
    VehicleTipe Type;
    PlayerDto(const std::string& username, VehicleTipe Type);
};

#endif
