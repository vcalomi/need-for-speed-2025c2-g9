#ifndef GAME_FINISHED_SERIALIZER_H
#define CHECKPOINT_SERIALIZER_H

#include "../Dto/game_finished.h"

#include "serializer.h"

class GameFinishedSerializer: public Serializer {
public:
    GameFinishedSerializer() {}
    std::vector<uint8_t> serialize(const Dto&) const override {
        std::vector<uint8_t> buffer(0);
        return buffer;
    }
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>&) const override {
        return std::make_shared<GameFinishedDto>();
    }
};

#endif
#include <memory>
