#include "race_finished_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <netinet/in.h>

#include "util.h"

std::vector<uint8_t> RaceFinishedSerializer::serialize(const Dto& dto) const {
    std::vector<uint8_t> buffer(0);
    return buffer;
}

std::shared_ptr<Dto> RaceFinishedSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    return std::make_shared<RaceFinishedDto>();
}
