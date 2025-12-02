#pragma once
#include <string>
#include <utility>

#include "event.h"

struct NpcHitEvent: public Event {
    std::string username;

    explicit NpcHitEvent(std::string username): username(std::move(username)) {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "NpcHitEvent"; }
};
