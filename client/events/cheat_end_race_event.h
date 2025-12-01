#pragma once
#include <string>

#include "event.h"

struct CheatEndRaceEvent: public Event {
    std::string username;

    explicit CheatEndRaceEvent(std::string username): username(username) {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "CheatEndRaceEvent"; }
};
