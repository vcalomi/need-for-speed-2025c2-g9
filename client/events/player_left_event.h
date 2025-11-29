#pragma once
#include <string>
#include <utility>
#include <vector>

#include "../../common/common_codes.h"
#include "../world/checkpoint.h"

#include "event.h"

struct PlayerLeftEvent: public Event {
    std::string username;

    explicit PlayerLeftEvent(std::string username): username(std::move(username)) {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "PlayerLeftEvent"; }
};
