#pragma once
#include <string>

#include "event.h"

struct CloseGameEvent: public Event {
    CloseGameEvent() {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "CloseGameEvent"; }
};
