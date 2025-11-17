#ifndef LOBBY_ROOM_DTO_H
#define LOBBY_ROOM_DTO_H

#include <string>
#include <vector>
#include "dto.h"

class RoomDto : public Dto {
public:
    std::string roomCode;
    uint8_t maxPlayers = 0;
    std::vector<std::string> rooms;

    explicit RoomDto(uint8_t code) : Dto(code) {}
    RoomDto(uint8_t code, const std::string& code_, uint8_t max_) : Dto(code), roomCode(code_), maxPlayers(max_) {}
    RoomDto(uint8_t code, const std::string& code_) : Dto(code), roomCode(code_) {}
    RoomDto(uint8_t code, std::vector<std::string> rooms_) : Dto(code), rooms(std::move(rooms_)) {}
};

#endif
