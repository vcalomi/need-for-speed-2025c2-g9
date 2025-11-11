#include "checkpoint_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <netinet/in.h>


std::vector<uint8_t> CheckpointSerializer::serialize(const Dto& dto) const {
    const CheckpointDto& checkpointDto = static_cast<const CheckpointDto&>(dto);
    size_t numCheckpoints = checkpointDto.checkpoints.size();
    std::vector<uint8_t> buffer(1 + numCheckpoints * 2 * sizeof(float));
    size_t pos = 0;

    buffer[pos++] = static_cast<uint8_t>(numCheckpoints);

    auto writeFloat = [&](float value) {
        std::memcpy(&buffer[pos], &value, sizeof(float));
        pos += sizeof(float);
    };

    for (const auto& checkpoint: checkpointDto.checkpoints) {
        writeFloat(checkpoint.x);
        writeFloat(checkpoint.y);
    }

    return buffer;
}

std::shared_ptr<Dto> CheckpointSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    uint8_t numCheckpoints = buffer[pos++];

    std::vector<Checkpoint> checkpoints;
    checkpoints.reserve(numCheckpoints);

    auto readFloat = [&]() {
        float value;
        std::memcpy(&value, &buffer[pos], sizeof(float));
        pos += sizeof(float);
        return value;
    };

    for (int i = 0; i < numCheckpoints; i++) {
        float x = readFloat();
        float y = readFloat();
        checkpoints.emplace_back(x, y);
    }
    return std::make_shared<CheckpointDto>(checkpoints);
}
