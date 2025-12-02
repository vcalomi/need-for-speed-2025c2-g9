#include "player_hit_npc.h"

#include "../common_codes.h"

PlayerHitNPCDto::PlayerHitNPCDto(const std::string& username, int id):
        Dto(uint8_t(ActionCode::SEND_PLAYER_HIT_NPC)), username(username), id(id) {}
