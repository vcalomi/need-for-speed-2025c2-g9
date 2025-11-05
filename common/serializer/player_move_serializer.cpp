#include "player_move_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <netinet/in.h>


std::vector<uint8_t> PlayerMoveSerializer::serialize(const Dto& dto) const {
    const PlayerMoveDto& playerMoveDto = static_cast<const PlayerMoveDto&>(dto);
    std::vector<uint8_t> buffer(2);
    size_t pos = 0;

    buffer[pos++] = playerMoveDto.id;
    buffer[pos++] = playerMoveDto.move;
    return buffer;
}

std::shared_ptr<Dto> PlayerMoveSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    uint8_t id = buffer[pos++];
    uint8_t move = buffer[pos++];
    return std::make_shared<PlayerMoveDto>(id, move);
}
