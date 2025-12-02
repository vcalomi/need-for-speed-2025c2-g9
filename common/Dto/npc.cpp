#include "npc.h"

#include "../common_codes.h"

NPCDto::NPCDto(int id, float x, float y):
        Dto(uint8_t(ActionCode::SEND_NPC)), 
        id(id),
        x(x),
        y(y) {}
