#ifndef NPC_SERIALIZER_H
#define NPC_SERIALIZER_H

#include "serializer.h"
#include "../Dto/npc.h"

class NPCSerializer : public Serializer {
public:
    NPCSerializer() {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
};

#endif
