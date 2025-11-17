#include "lobby_room_serializer.h"

#include <memory>
#include <utility>
#include <vector>

#include "util.h"

std::vector<uint8_t> RoomSerializer::serialize(const Dto& dto) const {
    const RoomDto& rd = static_cast<const RoomDto&>(dto);
    switch (code_) {
        case ActionCode::CREATE_ROOM: {
            size_t sz = SerializerUtils::STRING_LENGTH_SIZE + rd.roomCode.size() + 1;
            std::vector<uint8_t> buf(sz);
            size_t pos = 0;
            SerializerUtils::writeString(buf, pos, rd.roomCode);
            SerializerUtils::writeByte(buf, pos, rd.maxPlayers);
            return buf;
        }
        case ActionCode::JOIN_ROOM: {
            size_t sz = SerializerUtils::STRING_LENGTH_SIZE + rd.roomCode.size();
            std::vector<uint8_t> buf(sz);
            size_t pos = 0;
            SerializerUtils::writeString(buf, pos, rd.roomCode);
            return buf;
        }
        case ActionCode::LIST_ROOMS: {
            size_t buffer_size = SerializerUtils::calculateStringVectorSize(rd.rooms);
            std::vector<uint8_t> buffer(buffer_size);
            size_t pos = 0;
            SerializerUtils::writeStringVector(buffer, pos, rd.rooms);
            return buffer;
        }
        case ActionCode::ROOM_CREATED:
        case ActionCode::JOIN_OK: {
            return {};
        }
        default:
            return {};
    }
}

std::shared_ptr<Dto> RoomSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;
    switch (code_) {
        case ActionCode::CREATE_ROOM: {
            std::string room = SerializerUtils::readString(buffer, pos);
            uint8_t maxP = SerializerUtils::readByte(buffer, pos);
            return std::make_shared<RoomDto>(static_cast<uint8_t>(code_), room, maxP);
        }
        case ActionCode::JOIN_ROOM: {
            std::string room = SerializerUtils::readString(buffer, pos);
            return std::make_shared<RoomDto>(static_cast<uint8_t>(code_), room);
        }
        case ActionCode::LIST_ROOMS: {
            auto rooms = SerializerUtils::readStringVector(buffer, pos);
            return std::make_shared<RoomDto>(static_cast<uint8_t>(code_), std::move(rooms));
        }
        case ActionCode::ROOM_CREATED:
        case ActionCode::JOIN_OK: {
            return std::make_shared<RoomDto>(static_cast<uint8_t>(code_));
        }
        default:
            return std::make_shared<RoomDto>(static_cast<uint8_t>(code_));
    }
}
