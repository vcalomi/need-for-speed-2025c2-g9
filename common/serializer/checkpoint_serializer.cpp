#include "checkpoint_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "util.h"

std::vector<uint8_t> CheckpointSerializer::serialize(const Dto& dto) const {
    const CheckpointDto& checkpointDto = static_cast<const CheckpointDto&>(dto);
    std::vector<uint8_t> buffer(sizeof(int) + 2 * sizeof(float));
    size_t pos = 0;

    SerializerUtils::writeInt(buffer, pos, checkpointDto.id);
    SerializerUtils::writeFloat(buffer, pos, checkpointDto.x);
    SerializerUtils::writeFloat(buffer, pos, checkpointDto.y);

    return buffer;
}

std::shared_ptr<Dto> CheckpointSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    int id = SerializerUtils::readInt(buffer, pos);
    float x = SerializerUtils::readFloat(buffer, pos);
    float y = SerializerUtils::readFloat(buffer, pos);

    return std::make_shared<CheckpointDto>(id, x, y);
}
