#include "vehicle_collision_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "util.h"

std::vector<uint8_t> VehicleCollisionSerializer::serialize(const Dto& dto) const {
    const VehicleCollisionDto& vehicleCollisionDto = static_cast<const VehicleCollisionDto&>(dto);
    size_t username_len_1 = vehicleCollisionDto.vehicle1_username.length();
    size_t username_len_2 = vehicleCollisionDto.vehicle2_username.length();
    std::vector<uint8_t> buffer(2 + username_len_1 + username_len_2);
    size_t pos = 0;

    SerializerUtils::writeString(buffer, pos, vehicleCollisionDto.vehicle1_username);
    SerializerUtils::writeString(buffer, pos, vehicleCollisionDto.vehicle2_username);
    return buffer;
}

std::shared_ptr<Dto> VehicleCollisionSerializer::deserialize(
        const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    std::string vehicle1_username = SerializerUtils::readString(buffer, pos);
    std::string vehicle2_username = SerializerUtils::readString(buffer, pos);
    return std::make_shared<VehicleCollisionDto>(vehicle1_username, vehicle2_username);
}
