#include "vehicle_wall_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "util.h"

std::vector<uint8_t> VehicleWallCollisionSerializer::serialize(const Dto& dto) const {
    const VehicleWallCollisionDto& vehicleWallCollisionDto =
            static_cast<const VehicleWallCollisionDto&>(dto);
    std::vector<uint8_t> buffer(sizeof(int));
    size_t pos = 0;

    SerializerUtils::writeInt(buffer, pos, vehicleWallCollisionDto.vehicle_id);
    return buffer;
}

std::shared_ptr<Dto> VehicleWallCollisionSerializer::deserialize(
        const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    int vehicle_id = SerializerUtils::readInt(buffer, pos);
    return std::make_shared<VehicleWallCollisionDto>(vehicle_id);
}
