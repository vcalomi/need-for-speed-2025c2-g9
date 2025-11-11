#pragma once
#include <string>
#include <utility>
#include <vector>

#include "../../common/common_codes.h"
#include "../world/checkpoint.h"

#include "event.h"

struct CheckPointEvent: public Event {
    int id;
    float x;
    float y;

    explicit CheckPointEvent(int id, float x, float y): id(id), x(x), y(y) {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "CheckPointEvent"; }
};
