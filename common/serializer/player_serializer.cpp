#include "player_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <netinet/in.h>


std::vector<uint8_t> PlayerSerializer::serialize(const Dto& dto) const {
    const PlayerDto& playerDto = static_cast<const PlayerDto&>(dto);
    size_t username_len = playerDto.username.length();
    std::vector<uint8_t> buffer(1 + username_len + 1);
    size_t pos = 0;

    buffer[pos++] = static_cast<uint8_t>(username_len);
    for (char c: playerDto.username) {
        buffer[pos++] = static_cast<uint8_t>(c);
    }
    buffer[pos++] = static_cast<uint8_t>(playerDto.Type);
    return buffer;
}

std::shared_ptr<Dto> PlayerSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    uint8_t username_len = buffer[pos++];

    std::string username;
    for (int i = 0; i < username_len && pos < buffer.size(); i++) {
        username += static_cast<char>(buffer[pos++]);
    }
    uint8_t carType = buffer[pos++];
    return std::make_shared<PlayerDto>(username, static_cast<VehicleTipe>(carType));
}
