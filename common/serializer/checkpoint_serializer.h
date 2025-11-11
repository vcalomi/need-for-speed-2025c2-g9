#ifndef CHECKPOINT_SERIALIZER_H
#define CHECKPOINT_SERIALIZER_H

#include "../Dto/checkpoint.h"

#include "serializer.h"

class CheckpointSerializer: public Serializer {
public:
    CheckpointSerializer() {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
};

#endif
#include <memory>
#include <vector>
