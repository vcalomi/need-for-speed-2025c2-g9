#include "player_hit_npc_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <netinet/in.h>

#include "util.h"


std::vector<uint8_t> PlayerHitNPCSerializer::serialize(const Dto& dto) const {
    const PlayerHitNPCDto& playerHitNPCDto = static_cast<const PlayerHitNPCDto&>(dto);
    size_t username_len = playerHitNPCDto.username.length();
    std::vector<uint8_t> buffer(SerializerUtils::STRING_LENGTH_SIZE + username_len + 1 +
                                sizeof(int));
    size_t pos = 0;

    SerializerUtils::writeString(buffer, pos, playerHitNPCDto.username);
    SerializerUtils::writeInt(buffer, pos, playerHitNPCDto.id);
    return buffer;
}

std::shared_ptr<Dto> PlayerHitNPCSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;
    std::string username = SerializerUtils::readString(buffer, pos);
    int id = SerializerUtils::readInt(buffer, pos);
    return std::make_shared<PlayerHitNPCDto>(username, id);
}
