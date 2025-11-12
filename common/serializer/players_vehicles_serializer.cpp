#include "players_vehicles_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "util.h"

std::vector<uint8_t> PlayersVehiclesSerializer::serialize(const Dto& dto) const {
    const PlayersVehiclesDto& playersVehiclesDto = static_cast<const PlayersVehiclesDto&>(dto);
    std::vector<uint8_t> buffer(2 * sizeof(int));
    size_t pos = 0;

    SerializerUtils::writeInt(buffer, pos, playersVehiclesDto.vehicle1_id);
    SerializerUtils::writeInt(buffer, pos, playersVehiclesDto.vehicle2_id);
    return buffer;
}

std::shared_ptr<Dto> PlayersVehiclesSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;

    int vehicle1_id = SerializerUtils::readInt(buffer, pos);
    int vehicle2_id = SerializerUtils::readInt(buffer, pos);
    return std::make_shared<PlayersVehiclesDto>(vehicle1_id, vehicle2_id);
}
