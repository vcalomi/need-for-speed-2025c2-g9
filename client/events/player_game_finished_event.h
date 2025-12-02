#pragma once
#include <string>
#include <utility>

#include "event.h"

struct PlayerGameFinishedEvent: public Event {
    std::string username;
    float totalRaceTime;
    float totalPenalties;
    int finalPosition;


    explicit PlayerGameFinishedEvent(std::string username, float totalRaceTime,
                                     float totalPenalties, int finalPosition):
            username(std::move(username)),
            totalRaceTime(totalRaceTime),
            totalPenalties(totalPenalties),
            finalPosition(finalPosition) {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "PlayerGameFinishedEvent"; }
};
