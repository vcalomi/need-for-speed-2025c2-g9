#pragma once
#include <string>
#include <utility>
#include <vector>

#include "../../common/common_codes.h"
#include "../world/checkpoint.h"

#include "event.h"

struct VehicleExplodedEvent: public Event {
    std::string username;

    explicit VehicleExplodedEvent(std::string username): username(std::move(username)) {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "VehicleExplodedEvent"; }
};
