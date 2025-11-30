#include "infinite_health.h"

#include "../common_codes.h"

InfiniteHealthDto::InfiniteHealthDto():
        Dto(uint8_t(ActionCode::SEND_INFINITE_HEALTH)) {}
