#ifndef LOBBY_ROOM_STATE_DTO_H
#define LOBBY_ROOM_STATE_DTO_H

#include <string>
#include <vector>
#include "dto.h"

class RoomStateDto : public Dto {
public:
    uint8_t maxPlayers = 0;
    std::vector<std::string> players;
    bool started = false;

    explicit RoomStateDto(uint8_t code) : Dto(code) {}
};

#endif
