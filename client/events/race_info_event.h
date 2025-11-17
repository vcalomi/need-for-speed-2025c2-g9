#pragma once
#include <string>
#include <utility>
#include <vector>

#include "../../common/common_codes.h"
#include "../world/checkpoint.h"

#include "event.h"

struct RaceInfoEvent: public Event {
    std::string map;
    // std::vector<Checkpoint> checkpoints;


    explicit RaceInfoEvent(std::string map): map(std::move(map)) {}
    std::string GetType() const override { return Type(); }
    static std::string Type() { return "RaceInfoEvent"; }
};
