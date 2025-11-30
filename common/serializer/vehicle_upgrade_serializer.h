#ifndef VEHICLE_UPGRADE_SERIALIZER_H
#define VEHICLE_UPGRADE_SERIALIZER_H

#include "../Dto/vehicle_upgrade.h"

#include "serializer.h"

class VehicleUpgradeSerializer: public Serializer {
public:
    VehicleUpgradeSerializer() {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
};

#endif
