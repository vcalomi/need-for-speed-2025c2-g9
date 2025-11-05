#ifndef PLAYER_MOVE_SERIALIZER_H
#define PLAYER_MOVE_SERIALIZER_H

#include "../Dto/player_move.h"

#include "serializer.h"

class PlayerMoveSerializer: public Serializer {
private:
    const int size = 2;

public:
    PlayerMoveSerializer() {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
    int getSize() const override { return size; }
};

#endif
#include <memory>
#include <vector>
