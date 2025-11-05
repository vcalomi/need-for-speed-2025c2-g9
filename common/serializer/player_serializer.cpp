#include "player_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <netinet/in.h>


std::vector<uint8_t> PlayerSerializer::serialize(const Dto& dto) const {
    const PlayerDto& playerDto = static_cast<const PlayerDto&>(dto);
    std::vector<uint8_t> buffer(2);
    size_t pos = 0;

    buffer[pos++] = playerDto.id;
    buffer[pos++] = static_cast<uint8_t>(playerDto.Type);
    return buffer;
}

std::shared_ptr<Dto> PlayerSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    uint8_t id = buffer[pos++];
    uint8_t carType = buffer[pos++];
    return std::make_shared<PlayerDto>(id, static_cast<VehicleTipe>(carType));
}
