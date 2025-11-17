#include "player_left_serializer.h"

#include <vector>

#include "util.h"

std::vector<uint8_t> PlayerLeftSerializer::serialize(const Dto& dto) const {
    const PlayerLeftDto& playerLeftDto = static_cast<const PlayerLeftDto&>(dto);
    size_t size = SerializerUtils::STRING_LENGTH_SIZE + playerLeftDto.username.size();
    std::vector<uint8_t> buffer(size);
    size_t pos = 0;
    SerializerUtils::writeString(buffer, pos, playerLeftDto.username);
    return buffer;
}

std::shared_ptr<Dto> PlayerLeftSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;
    std::string username = SerializerUtils::readString(buffer, pos);
    return std::make_shared<PlayerLeftDto>(username);
}
