#include "end_race_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "util.h"

std::vector<uint8_t> EndRaceSerializer::serialize(const Dto& dto) const {
    const EndRaceDto& endRaceDto = static_cast<const EndRaceDto&>(dto);
    size_t size = endRaceDto.username.length();
    std::vector<uint8_t> buffer(SerializerUtils::STRING_LENGTH_SIZE + size);
    size_t pos = 0;

    SerializerUtils::writeString(buffer, pos, endRaceDto.username);
    return buffer;
}

std::shared_ptr<Dto> EndRaceSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;
    std::string username = SerializerUtils::readString(buffer, pos);
    return std::make_shared<EndRaceDto>(username);
}
