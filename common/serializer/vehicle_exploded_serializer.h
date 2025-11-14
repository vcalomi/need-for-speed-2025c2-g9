#ifndef VEHICLE_EXPLODED_SERIALIZER_H
#define VEHICLE_EXPLODED_SERIALIZER_H

#include "../Dto/vehicle_exploded.h"

#include "serializer.h"

class VehicleExplodedSerializer: public Serializer {
public:
    VehicleExplodedSerializer() {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
};

#endif
#include <memory>
