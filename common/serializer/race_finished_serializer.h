#ifndef RACE_FINISHED_SERIALIZER_H
#define RACE_FINISHED_SERIALIZER_H

#include <memory>
#include <vector>

#include "../Dto/race_finished.h"

#include "serializer.h"

class RaceFinishedSerializer: public Serializer {

public:
    RaceFinishedSerializer() {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
};

#endif
