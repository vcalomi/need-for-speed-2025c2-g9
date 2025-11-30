#include "vehicle_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <netinet/in.h>

#include "util.h"


std::vector<uint8_t> VehicleSerializer::serialize(const Dto& dto) const {
    const VehicleDto& vehicleDto = static_cast<const VehicleDto&>(dto);
    size_t username_len = vehicleDto.username.length();
    std::vector<uint8_t> buffer(SerializerUtils::STRING_LENGTH_SIZE + username_len + 4 * sizeof(float) + sizeof(bool));
    size_t pos = 0;

    SerializerUtils::writeString(buffer, pos, vehicleDto.username);
    SerializerUtils::writeFloat(buffer, pos, vehicleDto.x);
    SerializerUtils::writeFloat(buffer, pos, vehicleDto.y);
    SerializerUtils::writeFloat(buffer, pos, vehicleDto.rotation);
    SerializerUtils::writeFloat(buffer, pos, vehicleDto.speed);
    SerializerUtils::writeBool(buffer, pos, vehicleDto.isAboveBridge);
    return buffer;
}

std::shared_ptr<Dto> VehicleSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    std::string username = SerializerUtils::readString(buffer, pos);
    float x = SerializerUtils::readFloat(buffer, pos);
    float y = SerializerUtils::readFloat(buffer, pos);
    float rotation = SerializerUtils::readFloat(buffer, pos);
    float speed = SerializerUtils::readFloat(buffer, pos);
    bool isAboveBridge = SerializerUtils::readBool(buffer, pos);
    return std::make_shared<VehicleDto>(username, x, y, rotation, speed ,isAboveBridge);
}
