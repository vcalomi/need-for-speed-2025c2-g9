#include "vehicle_checkpoint_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "util.h"

std::vector<uint8_t> VehicleCheckpointSerializer::serialize(const Dto& dto) const {
    const VehicleCheckpointDto& vehicleCheckpointDto = static_cast<const VehicleCheckpointDto&>(dto);
    size_t username_len = vehicleCheckpointDto.username.length();
    std::vector<uint8_t> buffer(1 + username_len + sizeof(int));
    size_t pos = 0;

    SerializerUtils::writeString(buffer, pos, vehicleCheckpointDto.username);
    SerializerUtils::writeInt(buffer, pos, vehicleCheckpointDto.checkpointIndex);
    return buffer;
}

std::shared_ptr<Dto> VehicleCheckpointSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    std::string username = SerializerUtils::readString(buffer, pos);
    int checkpointIndex = SerializerUtils::readInt(buffer, pos);
    return std::make_shared<VehicleCheckpointDto>(username, checkpointIndex);
}
