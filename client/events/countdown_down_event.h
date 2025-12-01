#pragma once
#include <string>

#include "event.h"

struct CountdownDownEvent: public Event {
    explicit CountdownDownEvent() {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "CountdownDownEvent"; }
};
