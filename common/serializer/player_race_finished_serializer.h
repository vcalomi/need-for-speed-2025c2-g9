#ifndef PLAYER_RACE_FINISHED_SERIALIZER_H
#define PLAYER_RACE_FINISHED_SERIALIZER_H

#include <memory>
#include <vector>

#include "../Dto/player_race_finished.h"

#include "serializer.h"

class PlayerRaceFinishedSerializer: public Serializer {
public:
    PlayerRaceFinishedSerializer() {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
};

#endif
