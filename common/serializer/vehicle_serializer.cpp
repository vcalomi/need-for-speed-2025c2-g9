#include "vehicle_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <netinet/in.h>


std::vector<uint8_t> VehicleSerializer::serialize(const Dto& dto) const {
    const VehicleDto& vehicleDto = static_cast<const VehicleDto&>(dto);
    size_t username_len = vehicleDto.username.length();
    std::vector<uint8_t> buffer(1 + username_len + 3 * sizeof(float) + 2);
    size_t pos = 0;

    buffer[pos++] = static_cast<uint8_t>(username_len);
    for (char c: vehicleDto.username) {
        buffer[pos++] = static_cast<uint8_t>(c);
    }

    auto writeFloat = [&](float value) {
        std::memcpy(&buffer[pos], &value, sizeof(float));
        pos += sizeof(float);
    };
    writeFloat(vehicleDto.x);
    writeFloat(vehicleDto.y);
    writeFloat(vehicleDto.rotation);
    buffer[pos++] = vehicleDto.isAccelerating ? 1 : 0;
    buffer[pos++] = vehicleDto.isBraking ? 1 : 0;
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

    uint8_t username_len = buffer[pos++];
    std::string username;
    for (int i = 0; i < username_len && pos < buffer.size(); i++) {
        username += static_cast<char>(buffer[pos++]);
    }

    float x = readFloat();
    float y = readFloat();
    float rotation = readFloat();
    bool isAccelerating = buffer[pos++] != 0;
    bool isBraking = buffer[pos++] != 0;
    return std::make_shared<VehicleDto>(username, x, y, rotation, isAccelerating, isBraking);
}
