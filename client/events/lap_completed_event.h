#pragma once
#include <string>
#include <utility>
#include <vector>

#include "../../common/common_codes.h"
#include "../world/checkpoint.h"

#include "event.h"

struct LapCompletedEvent: public Event {
    std::string username;
    int current_lap;

    explicit LapCompletedEvent(std::string username, int current_lap):
            username(std::move(username)), current_lap(current_lap) {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "LapCompletedEvent"; }
};
