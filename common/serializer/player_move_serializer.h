#ifndef PLAYER_MOVE_SERIALIZER_H
#define PLAYER_MOVE_SERIALIZER_H

#include "../Dto/player_move.h"

#include "serializer.h"

class PlayerMoveSerializer: public Serializer {
public:
    PlayerMoveSerializer() {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
};

#endif
#include <memory>
#include <vector>
