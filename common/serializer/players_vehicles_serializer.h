#ifndef PLAYERS_VEHICLE_SERIALIZER_H
#define PLAYERS_VEHICLE_SERIALIZER_H

#include "../Dto/players_vehicles.h"

#include "serializer.h"

class PlayersVehiclesSerializer: public Serializer {
public:
    PlayersVehiclesSerializer() {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
};

#endif
#include <memory>
