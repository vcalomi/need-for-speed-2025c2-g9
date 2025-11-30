#include "vehicle_upgrade_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "util.h"

std::vector<uint8_t> VehicleUpgradeSerializer::serialize(const Dto& dto) const {
    const VehicleUpgradeDto& vehicleUpgradeDto = static_cast<const VehicleUpgradeDto&>(dto);
    std::vector<uint8_t> buffer(2 * sizeof(bool));
    size_t pos = 0;

    SerializerUtils::writeBool(buffer, pos, vehicleUpgradeDto.healthUpgrade);
    SerializerUtils::writeBool(buffer, pos, vehicleUpgradeDto.speedUpgrade);
    return buffer;
}

std::shared_ptr<Dto> VehicleUpgradeSerializer::deserialize(
        const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    bool healthUpgrade = SerializerUtils::readBool(buffer, pos);
    bool speedUpgrade = SerializerUtils::readBool(buffer, pos);
    return std::make_shared<VehicleUpgradeDto>(healthUpgrade, speedUpgrade);
}
