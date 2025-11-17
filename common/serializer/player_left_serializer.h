#ifndef PLAYER_LEFT_SERIALIZER_H
#define PLAYER_LEFT_SERIALIZER_H

#include "serializer.h"
#include "../Dto/player_left.h"

class PlayerLeftSerializer : public Serializer {
public:
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
    ~PlayerLeftSerializer() override = default;
};

#endif
