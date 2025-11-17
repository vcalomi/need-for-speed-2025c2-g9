#ifndef PLAYER_LEFT_DTO_H
#define PLAYER_LEFT_DTO_H

#include <string>

#include "dto.h"
#include "../common_codes.h"

class PlayerLeftDto : public Dto {
public:
    std::string username;
    explicit PlayerLeftDto(const std::string& username)
        : Dto(static_cast<uint8_t>(ActionCode::SEND_PLAYER_LEFT), username), username(username) {}
};

#endif
