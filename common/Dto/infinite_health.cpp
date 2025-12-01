#include "infinite_health.h"

#include "../common_codes.h"

InfiniteHealthDto::InfiniteHealthDto(const std::string& username):
        Dto(uint8_t(ActionCode::SEND_INFINITE_HEALTH)),
        username(username) {}
