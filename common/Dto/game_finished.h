#ifndef GAME_FINISHED_DTO_H
#define GAME_FINISHED_DTO_H

#include <string>

#include "dto.h"
#include "../common_codes.h"

class GameFinishedDto : public Dto {
public:
   GameFinishedDto()
        : Dto(static_cast<uint8_t>(ActionCode::SEND_GAME_FINISHED)) {}
};

#endif
