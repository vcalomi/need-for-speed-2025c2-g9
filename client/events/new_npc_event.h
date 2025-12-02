#pragma once
#include <string>

#include "event.h"

struct NewNpcEvent: public Event {
    int id;
    float x;
    float y;

    explicit NewNpcEvent(int id, float x, float y): id(id), x(x), y(y) {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "NewNpcEvent"; }
};
