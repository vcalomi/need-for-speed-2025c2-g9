#include "initial_race_map_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "util.h"

std::vector<uint8_t> InitialRaceMapSerializer::serialize(const Dto& dto) const {
    const InitialRaceMapDto& initialRaceMapDto = static_cast<const InitialRaceMapDto&>(dto);
    size_t map_len = initialRaceMapDto.mapName.length();
    std::vector<uint8_t> buffer(SerializerUtils::STRING_LENGTH_SIZE + map_len);
    size_t pos = 0;

    SerializerUtils::writeString(buffer, pos, initialRaceMapDto.mapName);
    return buffer;
}

std::shared_ptr<Dto> InitialRaceMapSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;
    std::string mapName = SerializerUtils::readString(buffer, pos);
    return std::make_shared<InitialRaceMapDto>(mapName);
}
