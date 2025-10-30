#ifndef VEHICLE_SERIALIZER_H
#define VEHICLE_SERIALIZER_H

#include "serializer.h"
#include "../Dto/vehicle.h"

class VehicleSerializer : public Serializer {
private:
    const int size = 13;

public:
    explicit VehicleSerializer() {}
    std::vector<uint8_t> serialize(Dto dto) const override;
    Dto deserialize(const std::vector<uint8_t>& buffer) const override;
    int getSize() const override { return size; }
};

#endif
