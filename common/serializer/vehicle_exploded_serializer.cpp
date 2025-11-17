#include "vehicle_exploded_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "util.h"

std::vector<uint8_t> VehicleExplodedSerializer::serialize(const Dto& dto) const {
    const VehicleExplodedDto& vehicleExplodedDto = static_cast<const VehicleExplodedDto&>(dto);
    size_t username_len = vehicleExplodedDto.username.length();
    std::vector<uint8_t> buffer(SerializerUtils::STRING_LENGTH_SIZE + username_len);
    size_t pos = 0;

    SerializerUtils::writeString(buffer, pos, vehicleExplodedDto.username);
    return buffer;
}

std::shared_ptr<Dto> VehicleExplodedSerializer::deserialize(
        const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    std::string username = SerializerUtils::readString(buffer, pos);
    return std::make_shared<VehicleExplodedDto>(username);
}
