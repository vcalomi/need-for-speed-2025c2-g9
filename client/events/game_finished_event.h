#pragma once
#include <string>

#include "event.h"

struct GameFinishedEvent: public Event {
    GameFinishedEvent() {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "GameFinishedEvent"; }
};
