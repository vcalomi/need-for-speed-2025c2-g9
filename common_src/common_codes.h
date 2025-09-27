#ifndef COMMON_CODES_H
#define COMMON_CODES_H

#include <cstdint>

enum class ActionCode : uint8_t {
    ACTIVATE_NITRO = 0x04,
    NITRO_ACTIVATED = 0x07,
    NITRO_DEACTIVATED = 0x08,
    SERVER_MSG = 0x10
};

#endif
