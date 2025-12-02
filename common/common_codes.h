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
    LIST_STATE = 0x13,
    SELECT_MAPS = 0x14,
    SELECT_MAPS_OK = 0x15,

    // Comandos del server/cliente

    SEND_PLAYER = 0x19,
    SEND_PLAYER_MOVE = 0x20,
    USERNAME_OK = 0x21,
    SEND_CHECKPOINTS = 0x22,
    SEND_VEHICLE_CHECKPOINT = 0x23,
    SEND_VEHICLES_COLLISION = 0x24,
    SEND_VEHICLE_WALL_COLLISION = 0x25,
    SEND_LAP_COMPLETED = 0x26,
    SEND_RACE_FINISHED = 0x27,
    SEND_VEHICLE_EXPLODED = 0x28,
    SEND_INITIAL_RACE_MAP = 0x29,
    SEND_PLAYER_RACE_FINISHED = 0x2A,
    SEND_PLAYER_LEFT = 0x30,
    SEND_VEHICLE_UPGRADE = 0x31,
    SEND_INFINITE_HEALTH = 0x32,
    SEND_END_RACE = 0x33,
    SEND_NPC = 0x34,
    SEND_PLAYER_HIT_NPC = 0x35,
    SEND_PLAYER_GAME_FINISHED = 0x36,
    SEND_GAME_FINISHED = 0x37,
    SEND_CLOSE_GAME = 0x38
};

enum class MoveMask : uint8_t {
    NONE = 0,
    ACCELERATE = 1 << 0,
    BRAKE = 1 << 1,
    TURN_LEFT = 1 << 2,
    TURN_RIGHT = 1 << 3
};

inline uint8_t operator|(MoveMask a, MoveMask b) {
    return static_cast<uint8_t>(a) | static_cast<uint8_t>(b);
}


enum class VehicleTipe : uint8_t {
    // Identificador de vehiculo
    FIAT_600 = 0x40,
    FERRARI_F40 = 0x41,
    PORSCHE_911 = 0x42,
    SEDAN = 0x43,
    JEEP = 0x44,
    F100 = 0x45,
    TRUCK = 0x46
};

#endif
