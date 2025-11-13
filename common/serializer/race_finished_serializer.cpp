#include "race_finished_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <netinet/in.h>

#include "util.h"

std::vector<uint8_t> RaceFinishedSerializer::serialize(const Dto& dto) const {
    const RaceFinishedDto& raceFinishedDto = static_cast<const RaceFinishedDto&>(dto);
    size_t username_len = raceFinishedDto.username.length();
    std::vector<uint8_t> buffer(1 + username_len);
    size_t pos = 0;

    SerializerUtils::writeString(buffer, pos, raceFinishedDto.username);
    return buffer;
}

std::shared_ptr<Dto> RaceFinishedSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    std::string username = SerializerUtils::readString(buffer, pos);
    return std::make_shared<RaceFinishedDto>(username);
}
