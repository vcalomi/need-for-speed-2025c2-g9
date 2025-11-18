#include "lobby_room_state_serializer.h"

#include <memory>
#include <vector>

#include "util.h"

std::vector<uint8_t> RoomStateSerializer::serialize(const Dto& dto) const {
    const RoomStateDto& rs = static_cast<const RoomStateDto&>(dto);
    switch (code_) {
        case ActionCode::LIST_PLAYERS: {
            size_t buffer_size = 1 + SerializerUtils::calculateStringVectorSize(rs.players);
            std::vector<uint8_t> buffer(buffer_size);
            size_t pos = 0;
            SerializerUtils::writeByte(buffer, pos, rs.maxPlayers);
            SerializerUtils::writeStringVector(buffer, pos, rs.players);
            return buffer;
        }
        case ActionCode::LIST_STATE: {
            std::vector<uint8_t> buf(1);
            size_t pos = 0;
            SerializerUtils::writeBool(buf, pos, rs.started);
            return buf;
        }
        case ActionCode::START_GAME: {
            return {};
        }
        default:
            return {};
    }
}

std::shared_ptr<Dto> RoomStateSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;
    switch (code_) {
        case ActionCode::LIST_PLAYERS: {
            auto dto = std::make_shared<RoomStateDto>(static_cast<uint8_t>(code_));
            dto->maxPlayers = SerializerUtils::readByte(buffer, pos);
            dto->players = SerializerUtils::readStringVector(buffer, pos);
            return dto;
        }
        case ActionCode::LIST_STATE: {
            auto dto = std::make_shared<RoomStateDto>(static_cast<uint8_t>(code_));
            dto->started = SerializerUtils::readBool(buffer, pos);
            return dto;
        }
        case ActionCode::START_GAME: {
            return std::make_shared<RoomStateDto>(static_cast<uint8_t>(code_));
        }
        default:
            return std::make_shared<RoomStateDto>(static_cast<uint8_t>(code_));
    }
}
