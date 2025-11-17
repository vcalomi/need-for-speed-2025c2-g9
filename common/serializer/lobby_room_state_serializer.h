#ifndef LOBBY_ROOM_STATE_SERIALIZER_H
#define LOBBY_ROOM_STATE_SERIALIZER_H

#include "serializer.h"
#include "../common_codes.h"
#include "../Dto/lobby_room_state.h"

class RoomStateSerializer : public Serializer {
    ActionCode code_;
public:
    explicit RoomStateSerializer(ActionCode code) : code_(code) {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
};

#endif
