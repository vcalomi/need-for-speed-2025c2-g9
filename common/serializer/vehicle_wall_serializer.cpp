#include "vehicle_wall_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "util.h"

std::vector<uint8_t> VehicleWallSerializer::serialize(const Dto& dto) const {
    const VehicleWallDto& vehicleWallDto = static_cast<const VehicleWallDto&>(dto);
    std::vector<uint8_t> buffer(2 * sizeof(int));
    size_t pos = 0;

    SerializerUtils::writeInt(buffer, pos, vehicleWallDto.vehicle_id);
    return buffer;
}

std::shared_ptr<Dto> VehicleWallSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    int vehicle_id = SerializerUtils::readInt(buffer, pos);
    return std::make_shared<VehicleWallDto>(vehicle_id);
}
