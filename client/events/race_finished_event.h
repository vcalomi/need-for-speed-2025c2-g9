#pragma once
#include <string>
#include <utility>
#include <vector>

#include "event.h"

struct RaceFinishedEvent: public Event {
    RaceFinishedEvent() {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "RaceFinishedEvent"; }
};
