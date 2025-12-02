#include "player_race_finished_serializer.h"

#include <cstring>
#include <iostream>
#include <memory>
#include <vector>

#include <netinet/in.h>

#include "util.h"

std::vector<uint8_t> PlayerRaceFinishedSerializer::serialize(const Dto& dto) const {
    const PlayerRaceFinishedDto& raceFinishedDto = static_cast<const PlayerRaceFinishedDto&>(dto);
    size_t username_len = raceFinishedDto.username.length();
    std::vector<uint8_t> buffer(SerializerUtils::STRING_LENGTH_SIZE + username_len + sizeof(float) +
                                sizeof(int));
    size_t pos = 0;

    SerializerUtils::writeString(buffer, pos, raceFinishedDto.username);
    SerializerUtils::writeFloat(buffer, pos, raceFinishedDto.finishTime);
    SerializerUtils::writeInt(buffer, pos, raceFinishedDto.finalPosition);
    return buffer;
}

std::shared_ptr<Dto> PlayerRaceFinishedSerializer::deserialize(
        const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    std::string username = SerializerUtils::readString(buffer, pos);
    float finishTime = SerializerUtils::readFloat(buffer, pos);
    int finalPosition = SerializerUtils::readInt(buffer, pos);
    return std::make_shared<PlayerRaceFinishedDto>(username, finishTime, finalPosition);
}