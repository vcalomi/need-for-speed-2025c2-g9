#include "player_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <netinet/in.h>

#include "util.h"


std::vector<uint8_t> PlayerSerializer::serialize(const Dto& dto) const {
    const PlayerDto& playerDto = static_cast<const PlayerDto&>(dto);
    size_t username_len = playerDto.username.length();
    std::vector<uint8_t> buffer(SerializerUtils::STRING_LENGTH_SIZE + username_len + 1 + sizeof(float));
    size_t pos = 0;

    SerializerUtils::writeString(buffer, pos, playerDto.username);
    SerializerUtils::writeByte(buffer, pos, static_cast<uint8_t>(playerDto.Type));
    SerializerUtils::writeFloat(buffer, pos, playerDto.car_hp);
    return buffer;
}

std::shared_ptr<Dto> PlayerSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    std::string username = SerializerUtils::readString(buffer, pos);
    uint8_t carType = SerializerUtils::readByte(buffer, pos);
    float car_hp = SerializerUtils::readFloat(buffer, pos);
    return std::make_shared<PlayerDto>(username, static_cast<VehicleTipe>(carType), car_hp);
}
