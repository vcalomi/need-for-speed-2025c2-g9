#ifndef LOBBY_AUTH_DTO_H
#define LOBBY_AUTH_DTO_H

#include <string>
#include <vector>
#include "dto.h"

class AuthDto : public Dto {
public:
    std::string username;
    std::string errorMsg;

    AuthDto(uint8_t code) : Dto(code) {}
    AuthDto(uint8_t code, const std::string& username_) : Dto(code), username(username_) {}
};

#endif
