#pragma once
#include <string>
#include <utility>
#include <vector>

#include "../../common/common_codes.h"
#include "../world/checkpoint.h"

#include "event.h"

struct CheckPointsEvent: public Event {
    std::vector<Checkpoint> checkpoints;

    explicit CheckPointsEvent(std::vector<Checkpoint> checkpoints):
            checkpoints(std::move(checkpoints)) {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "CheckPointsEvent"; }
};
