#ifndef PLAYER_SERIALIZER_H
#define PLAYER_SERIALIZER_H

#include "../Dto/player.h"

#include "serializer.h"

class PlayerSerializer: public Serializer {
private:
    const int size = 2;

public:
    PlayerSerializer() {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
    int getSize() const override { return size; }
};

#endif
#include <memory>
#include <vector>
