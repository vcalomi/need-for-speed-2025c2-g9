#ifndef VEHICLE_CHECKPOINT_SERIALIZER_H
#define VEHICLE_CHECKPOINT_SERIALIZER_H

#include "../Dto/vehicle_checkpoint.h"

#include "serializer.h"

class VehicleCheckpointSerializer: public Serializer {
public:
    VehicleCheckpointSerializer() {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
};

#endif
#include <memory>
