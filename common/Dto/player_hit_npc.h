#ifndef PLAYER_HIT_NPC_DTO_H
#define PLAYER_HIT_NPC_DTO_H
#include <string>

#include "../common_codes.h"

#include "dto.h"

class PlayerHitNPCDto: public Dto {
public:
    std::string username;

    PlayerHitNPCDto(const std::string& username);
};

#endif
