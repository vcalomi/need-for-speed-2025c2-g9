#include "vehicle_wall_collision_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "util.h"

std::vector<uint8_t> VehicleWallCollisionSerializer::serialize(const Dto& dto) const {
    const VehicleWallCollisionDto& vehicleWallCollisionDto =
            static_cast<const VehicleWallCollisionDto&>(dto);
    size_t username_len = vehicleWallCollisionDto.username.length();
    std::vector<uint8_t> buffer(1 + username_len + sizeof(float));
    size_t pos = 0;

    SerializerUtils::writeString(buffer, pos, vehicleWallCollisionDto.username);
    SerializerUtils::writeFloat(buffer, pos, vehicleWallCollisionDto.vehicle_new_hp);
    return buffer;
}

std::shared_ptr<Dto> VehicleWallCollisionSerializer::deserialize(
        const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    std::string username = SerializerUtils::readString(buffer, pos);
    float vehicle_new_hp = SerializerUtils::readFloat(buffer, pos);
    return std::make_shared<VehicleWallCollisionDto>(username, vehicle_new_hp);
}
