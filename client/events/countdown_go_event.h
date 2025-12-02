#pragma once
#include <string>

#include "event.h"

struct CountdownGoEvent: public Event {
    CountdownGoEvent() {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "CountdownGoEvent"; }
};
