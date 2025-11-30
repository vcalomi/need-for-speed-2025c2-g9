#ifndef END_RACE_SERIALIZER_H
#define END_RACE_SERIALIZER_H

#include <memory>
#include <vector>

#include "../Dto/end_race.h"

#include "serializer.h"

class EndRaceSerializer: public Serializer {

public:
    EndRaceSerializer() {}
    std::vector<uint8_t> serialize(const Dto&) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>&) const override;
};

#endif
