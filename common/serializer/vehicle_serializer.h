#ifndef VEHICLE_SERIALIZER_H
#define VEHICLE_SERIALIZER_H

#include "../Dto/vehicle.h"

#include "serializer.h"

class VehicleSerializer: public Serializer {
private:
    const int size = 13;

public:
    VehicleSerializer() {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
    int getSize() const override { return size; }
};

#endif
#include <memory>
#include <vector>
