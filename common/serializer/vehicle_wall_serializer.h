#ifndef VEHICLE_WALL_SERIALIZER_H
#define VEHICLE_WALL_SERIALIZER_H

#include "../Dto/vehicle_wall.h"

#include "serializer.h"

class VehicleWallCollisionSerializer: public Serializer {
public:
    VehicleWallCollisionSerializer() {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
};

#endif
#include <memory>
#include <vector>
