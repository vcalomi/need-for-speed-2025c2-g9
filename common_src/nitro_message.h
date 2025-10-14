#include <cstdint>
#ifndef NITRO_MESSAGE_H
#define NITRO_MESSAGE_H

#include "common_codes.h"

struct NitroMessage {
    ActionCode msgCode;
    uint16_t carsWithNitro;
    ActionCode nitroState;
};

#endif
