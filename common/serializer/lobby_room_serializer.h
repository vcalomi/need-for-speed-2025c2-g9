#ifndef LOBBY_ROOM_SERIALIZER_H
#define LOBBY_ROOM_SERIALIZER_H

#include "serializer.h"
#include "../common_codes.h"
#include "../Dto/lobby_room.h"

class RoomSerializer : public Serializer {
    ActionCode code_;
public:
    explicit RoomSerializer(ActionCode code) : code_(code) {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
};

#endif
