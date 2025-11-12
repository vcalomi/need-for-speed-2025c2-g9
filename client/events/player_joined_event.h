#pragma once
#include <string>
#include <utility>
#include <vector>

#include "../../common/common_codes.h"
#include "../world/checkpoint.h"

#include "event.h"

struct PlayerJoinedEvent: public Event {
    std::string username;
    VehicleTipe type;
    bool isLocal = false;

    explicit PlayerJoinedEvent(const std::string& username, VehicleTipe type, bool isLocal = false):
            username(username), type(type), isLocal(isLocal) {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "PlayerJoinedEvent"; }
};
