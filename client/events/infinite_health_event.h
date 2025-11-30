#pragma once
#include <string>

#include "event.h"

struct InfiniteHealthEvent: public Event {

    InfiniteHealthEvent() {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "InfiniteHealthEvent"; }
};
