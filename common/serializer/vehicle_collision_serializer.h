#ifndef VEHICLE_COLLISION_SERIALIZER_H
#define VEHICLE_COLLISION_SERIALIZER_H

#include "../Dto/vehicle_collision.h"

#include "serializer.h"

class VehicleCollisionSerializer: public Serializer {
public:
    VehicleCollisionSerializer() {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
};

#endif
#include <memory>
