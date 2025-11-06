#include "vehicle_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <netinet/in.h>


std::vector<uint8_t> VehicleSerializer::serialize(const Dto& dto) const {
    const VehicleDto& vehicleDto = static_cast<const VehicleDto&>(dto);
    std::vector<uint8_t> buffer(sizeof(uint8_t) + 3 * sizeof(float));
    size_t pos = 0;

    auto writeFloat = [&](float value) {
        std::memcpy(&buffer[pos], &value, sizeof(float));
        pos += sizeof(float);
    };

    buffer[pos++] = vehicleDto.id;
    writeFloat(vehicleDto.x);
    writeFloat(vehicleDto.y);
    writeFloat(vehicleDto.rotation);

    return buffer;
}

std::shared_ptr<Dto> VehicleSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    auto readFloat = [&]() {
        float value;
        std::memcpy(&value, &buffer[pos], sizeof(float));
        pos += sizeof(float);
        return value;
    };

    uint8_t id = buffer[pos++];
    float x = readFloat();
    float y = readFloat();
    float rotation = readFloat();

    return std::make_shared<VehicleDto>(id, x, y, rotation);
}
