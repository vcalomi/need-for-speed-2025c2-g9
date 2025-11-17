#pragma once
#include <string>
#include <utility>
#include <vector>

#include "../../common/common_codes.h"
#include "../world/checkpoint.h"

#include "event.h"

struct PlayerRaceFinishedEvent: public Event {
    std::string username;
    float finishTime;
    int finalPosition;

    explicit PlayerRaceFinishedEvent(std::string username, float finishTime, int finalPosition):
            username(std::move(username)), finishTime(finishTime), finalPosition(finalPosition) {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "PlayerRaceFinishedEvent"; }
};
