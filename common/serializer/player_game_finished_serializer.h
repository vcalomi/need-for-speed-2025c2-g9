#ifndef PLAYER_GAME_FINISHED_SERIALIZER_H
#define PLAYER_GAME_FINISHED_SERIALIZER_H

#include "serializer.h"
#include "../Dto/player_game_finished.h"

class PlayerGameFinishedSerializer : public Serializer {
public:
    PlayerGameFinishedSerializer() {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
};

#endif
