#include "player_move_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <netinet/in.h>

#include "util.h"


std::vector<uint8_t> PlayerMoveSerializer::serialize(const Dto& dto) const {
    const PlayerMoveDto& playerMoveDto = static_cast<const PlayerMoveDto&>(dto);
    size_t username_len = playerMoveDto.username.length();
    std::vector<uint8_t> buffer(1 + username_len + 1);
    size_t pos = 0;

    SerializerUtils::writeString(buffer, pos, playerMoveDto.username);
    SerializerUtils::writeByte(buffer, pos, playerMoveDto.move);
    return buffer;
}

std::shared_ptr<Dto> PlayerMoveSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    std::string username = SerializerUtils::readString(buffer, pos);
    uint8_t move = SerializerUtils::readByte(buffer, pos);
    return std::make_shared<PlayerMoveDto>(username, move);
}
