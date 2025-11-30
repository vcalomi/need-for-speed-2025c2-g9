#pragma once
#include <string>

#include "event.h"

struct CheatEndRaceEvent: public Event {

    CheatEndRaceEvent() {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "CheatEndRaceEvent"; }
};
