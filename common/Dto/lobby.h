#ifndef LOBBY_DTO_H
#define LOBBY_DTO_H

#include <string>

#include "../common_codes.h"

#include "dto.h"

class ListRoomsDto: public Dto {
public:
    ListRoomsDto(): Dto(static_cast<uint8_t>(ActionCode::LIST_ROOMS)) {}
};

class StartGameDto: public Dto {
public:
    StartGameDto(): Dto(static_cast<uint8_t>(ActionCode::START_GAME)) {}
};

class CreateRoomDto: public Dto {
public:
    std::string roomName;
    explicit CreateRoomDto(const std::string& name):
            Dto(static_cast<uint8_t>(ActionCode::CREATE_ROOM)), roomName(name) {}
};

class JoinRoomDto: public Dto {
public:
    std::string roomName;
    explicit JoinRoomDto(const std::string& name):
            Dto(static_cast<uint8_t>(ActionCode::JOIN_ROOM)), roomName(name) {}
};

#endif
