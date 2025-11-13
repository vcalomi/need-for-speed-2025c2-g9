#include "lap_completed_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <netinet/in.h>

#include "util.h"

std::vector<uint8_t> LapCompletedSerializer::serialize(const Dto& dto) const {
    const LapCompletedDto& lapCompletedDto = static_cast<const LapCompletedDto&>(dto);
    size_t username_len = lapCompletedDto.username.length();
    std::vector<uint8_t> buffer(1 + username_len + 1 + sizeof(int));
    size_t pos = 0;

    SerializerUtils::writeString(buffer, pos, lapCompletedDto.username);
    SerializerUtils::writeInt(buffer, pos, lapCompletedDto.current_lap);
    return buffer;
}

std::shared_ptr<Dto> LapCompletedSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    std::string username = SerializerUtils::readString(buffer, pos);
    int current_lap = SerializerUtils::readInt(buffer, pos);
    return std::make_shared<LapCompletedDto>(username, current_lap);
}
