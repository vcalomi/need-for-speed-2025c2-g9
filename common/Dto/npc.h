#ifndef NPC_DTO_H
#define NPC_DTO_H
#include <string>

#include "../common_codes.h"

#include "dto.h"

class NPCDto: public Dto {
public:
    int id;
    float x;
    float y;

    NPCDto(int id, float x, float y);
};

#endif
