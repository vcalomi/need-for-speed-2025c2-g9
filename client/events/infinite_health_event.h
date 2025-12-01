#pragma once
#include <string>

#include "event.h"

struct InfiniteHealthEvent: public Event {
    std::string username;

    explicit InfiniteHealthEvent(std::string username): username(username) {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "InfiniteHealthEvent"; }
};
