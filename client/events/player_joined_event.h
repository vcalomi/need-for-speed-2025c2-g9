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
    float health;

    explicit PlayerJoinedEvent(const std::string& username, VehicleTipe type, bool isLocal = false,
                               float health = 100.0f):
            username(username), type(type), isLocal(isLocal), health(health) {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "PlayerJoinedEvent"; }
};
