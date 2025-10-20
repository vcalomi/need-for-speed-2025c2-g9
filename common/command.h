#pragma once
#include <cstdint>


// Comandos de juego que llegan desde el cliente
enum : uint8_t {
    CMD_NITRO = 0x04,
};

struct Command {
    int id;
    uint8_t code;
};
