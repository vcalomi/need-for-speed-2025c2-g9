#include "vehicle_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <netinet/in.h>

std::vector<uint8_t> VehicleSerializer::serialize(const Dto& dto) const {
    const VehicleDto& vehicleDto = static_cast<const VehicleDto&>(dto);
    std::vector<uint8_t> buffer(13);
    size_t pos = 0;

    auto writeFloat = [&](float value) {
        uint32_t floatToInt = uint32_t(value * 100.0f);
        uint32_t parsed = htonl(floatToInt);
        memcpy(&buffer[pos], &parsed, sizeof(parsed));
        pos += sizeof(parsed);
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
        uint32_t value;
        memcpy(&value, &buffer[pos], sizeof(value));
        pos += sizeof(value);
        uint32_t parsed = ntohl(value);
        return float(parsed) / 100.0f;
    };

    uint8_t id = buffer[pos++];
    float x = readFloat();
    float y = readFloat();
    float rotation = readFloat();
    return std::make_shared<VehicleDto>(id, x, y, rotation);
}
