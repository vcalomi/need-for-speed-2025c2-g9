#ifndef VEHICLE_SERIALIZER_H
#define VEHICLE_SERIALIZER_H

#include "../Dto/vehicle.h"

#include "serializer.h"

class VehicleSerializer: public Serializer {
public:
    VehicleSerializer() {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
};

#endif
#include <memory>
#include <vector>
