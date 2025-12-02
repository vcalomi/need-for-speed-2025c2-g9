#ifndef CLOSE_GAME_DTO_H
#define CLOSE_GAME_DTO_H

#include <string>

#include "../common_codes.h"

#include "dto.h"

class CloseGameDto: public Dto {
public:
    CloseGameDto(): Dto(static_cast<uint8_t>(ActionCode::SEND_CLOSE_GAME)) {}
};

#endif
