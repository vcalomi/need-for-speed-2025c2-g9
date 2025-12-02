#ifndef PLAYER_HIT_NPC_SERIALIZER_H
#define PLAYER_HIT_NPC_SERIALIZER_H

#include "serializer.h"
#include "../Dto/player_hit_npc.h"

class PlayerHitNPCSerializer : public Serializer {
public:
    PlayerHitNPCSerializer() {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
};

#endif
