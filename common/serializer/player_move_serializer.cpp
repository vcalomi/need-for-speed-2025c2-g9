#include "player_move_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <netinet/in.h>


std::vector<uint8_t> PlayerMoveSerializer::serialize(const Dto& dto) const {
    const PlayerMoveDto& playerMoveDto = static_cast<const PlayerMoveDto&>(dto);
    size_t username_len = playerMoveDto.username.length();
    std::vector<uint8_t> buffer(1 + username_len + 1);
    size_t pos = 0;

    buffer[pos++] = static_cast<uint8_t>(username_len);
    for (char c: playerMoveDto.username) {
        buffer[pos++] = static_cast<uint8_t>(c);
    }
    buffer[pos++] = playerMoveDto.move;
    return buffer;
}

std::shared_ptr<Dto> PlayerMoveSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    uint8_t username_len = buffer[pos++];

    std::string username;
    for (int i = 0; i < username_len && pos < buffer.size(); i++) {
        username += static_cast<char>(buffer[pos++]);
    }
    uint8_t move = buffer[pos++];
    return std::make_shared<PlayerMoveDto>(username, move);
}
