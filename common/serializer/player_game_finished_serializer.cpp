#include "player_game_finished_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <netinet/in.h>

#include "util.h"


std::vector<uint8_t> PlayerGameFinishedSerializer::serialize(const Dto& dto) const {
    const PlayerGameFinishedDto& playerGameFinishedDto = static_cast<const PlayerGameFinishedDto&>(dto);
    size_t username_len = playerGameFinishedDto.username.length();
    std::vector<uint8_t> buffer(SerializerUtils::STRING_LENGTH_SIZE + username_len + 1 + sizeof(float) * 2 + sizeof(int));
    size_t pos = 0;

    SerializerUtils::writeString(buffer, pos, playerGameFinishedDto.username);
    SerializerUtils::writeFloat(buffer, pos, playerGameFinishedDto.totalRaceTime);
    SerializerUtils::writeFloat(buffer, pos, playerGameFinishedDto.totalPenalties);
    SerializerUtils::writeInt(buffer, pos, playerGameFinishedDto.finalPosition);
    return buffer;
}

std::shared_ptr<Dto> PlayerGameFinishedSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;
    
    std::string username = SerializerUtils::readString(buffer, pos);
    float totalRaceTime = SerializerUtils::readFloat(buffer, pos);
    float totalPenalties = SerializerUtils::readFloat(buffer, pos);
    int finalPosition = SerializerUtils::readInt(buffer, pos);
    return std::make_shared<PlayerGameFinishedDto>(username, totalRaceTime, totalPenalties, finalPosition);
}
