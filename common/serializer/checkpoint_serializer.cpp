#include "checkpoint_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <netinet/in.h>
#include <arpa/inet.h>

std::vector<uint8_t> CheckpointSerializer::serialize(const Dto& dto) const {
    const CheckpointDto& checkpointDto = static_cast<const CheckpointDto&>(dto);
    std::vector<uint8_t> buffer(sizeof(int) + 2 * sizeof(float));
    size_t pos = 0;

    auto writeInt = [&](int value) {
        value = htonl(value);
        std::memcpy(&buffer[pos], &value, sizeof(int));
        pos += sizeof(int);
    };

    auto writeFloat = [&](float value) {
        std::memcpy(&buffer[pos], &value, sizeof(float));
        pos += sizeof(float);
    };

    writeInt(checkpointDto.id);
    writeFloat(checkpointDto.x);
    writeFloat(checkpointDto.y);

    return buffer;
}

std::shared_ptr<Dto> CheckpointSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    auto readInt = [&]() {
        int value;
        std::memcpy(&value, &buffer[pos], sizeof(int));
        pos += sizeof(int);
        return ntohl(value);
    };

    auto readFloat = [&]() {
        float value;
        std::memcpy(&value, &buffer[pos], sizeof(float));
        pos += sizeof(float);
        return value;
    };

    int id = readInt();
    float x = readFloat();
    float y = readFloat();

    return std::make_shared<CheckpointDto>(id, x, y);
}
