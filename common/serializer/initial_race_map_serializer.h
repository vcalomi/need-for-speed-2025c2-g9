#ifndef INITIAL_RACE_MAP_SERIALIZER_H
#define INITIAL_RACE_MAP_SERIALIZER_H

#include "../Dto/initial_race_map.h"

#include "serializer.h"

class InitialRaceMapSerializer: public Serializer {
public:
    InitialRaceMapSerializer() {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
};

#endif
#include <memory>
