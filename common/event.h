#pragma once
#include <cstdint>


enum : uint8_t {
    MSG_EVT = 0x10,
    EVT_HIT = 0x07,  // Nitro activado en algún auto
    EVT_OUT = 0x08,  // Nitro expirado en algún auto
};


struct Event {
    uint16_t cars_with_nitro;
    uint8_t kind;  // EVT_HIT o EVT_OUT
};
