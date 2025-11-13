#ifndef LAP_COMPLETED_SERIALIZER_H
#define LAP_COMPLETED_SERIALIZER_H

#include <memory>
#include <vector>

#include "../Dto/lap_completed.h"

#include "serializer.h"

class LapCompletedSerializer: public Serializer {

public:
    LapCompletedSerializer() {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
};

#endif
