#ifndef COMMON_CODES_H
#define COMMON_CODES_H

#include <cstdint>

enum class ActionCode : uint8_t {
    // Comandos del Lobby
    SEND_USERNAME = 0x01,
    CREATE_ROOM = 0x02,
    ROOM_CREATED = 0x3,
    JOIN_ROOM = 0x04,
    JOIN_OK = 0x5,
    LIST_ROOMS = 0x06,
    
    // Comandos de Sala
    CHOOSE_CAR = 0x07,
    CHOOSE_CAR_OK = 0x08,
    START_GAME = 0x09,

    SEND_ERROR_MSG = 0x10,
    SEND_CARS = 0x11,
    LIST_PLAYERS = 0x12,
    LIST_STATE = 0x13
};

#endif