#include "vehicle_upgrade_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "util.h"

std::vector<uint8_t> VehicleUpgradeSerializer::serialize(const Dto& dto) const {
    const VehicleUpgradeDto& vehicleUpgradeDto = static_cast<const VehicleUpgradeDto&>(dto);
    size_t username_len = vehicleUpgradeDto.username.length();
    std::vector<uint8_t> buffer(SerializerUtils::STRING_LENGTH_SIZE + username_len + 2 * sizeof(bool));
    size_t pos = 0;

    SerializerUtils::writeString(buffer, pos, vehicleUpgradeDto.username);
    SerializerUtils::writeBool(buffer, pos, vehicleUpgradeDto.healthUpgrade);
    SerializerUtils::writeBool(buffer, pos, vehicleUpgradeDto.speedUpgrade);
    return buffer;
}

std::shared_ptr<Dto> VehicleUpgradeSerializer::deserialize(
        const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;
    
    std::string username = SerializerUtils::readString(buffer, pos);
    bool healthUpgrade = SerializerUtils::readBool(buffer, pos);
    bool speedUpgrade = SerializerUtils::readBool(buffer, pos);
    return std::make_shared<VehicleUpgradeDto>(username, healthUpgrade, speedUpgrade);
}
