#include "npc_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <netinet/in.h>

#include "util.h"


std::vector<uint8_t> NPCSerializer::serialize(const Dto& dto) const {
    const NPCDto& npcDto = static_cast<const NPCDto&>(dto);
    std::vector<uint8_t> buffer(sizeof(int) + 2 * sizeof(float));
    size_t pos = 0;

    SerializerUtils::writeInt(buffer, pos, npcDto.id);
    SerializerUtils::writeFloat(buffer, pos, npcDto.x);
    SerializerUtils::writeFloat(buffer, pos, npcDto.y);
    return buffer;
}

std::shared_ptr<Dto> NPCSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;
    int id = SerializerUtils::readInt(buffer, pos);
    float x = SerializerUtils::readFloat(buffer, pos);
    float y = SerializerUtils::readFloat(buffer, pos);
    return std::make_shared<NPCDto>(id, x, y);
}
