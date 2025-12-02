#ifndef GAME_FINISHED_SERIALIZER_H
#define CHECKPOINT_SERIALIZER_H

#include "../Dto/close_game.h"

#include "serializer.h"

class CloseGameSerializer: public Serializer {
public:
    CloseGameSerializer() {}
    std::vector<uint8_t> serialize(const Dto&) const override {
        std::vector<uint8_t> buffer(0);
        return buffer;
    }
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>&) const override {
        return std::make_shared<CloseGameDto>();
    }
};

#endif
#include <memory>
