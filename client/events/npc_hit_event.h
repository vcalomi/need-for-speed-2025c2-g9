#pragma once
#include <string>
#include <utility>

#include "event.h"

struct NpcHitEvent: public Event {
    std::string username;
    int npcId;

    explicit NpcHitEvent(std::string username, int npcId):
            username(std::move(username)), npcId(npcId) {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "NpcHitEvent"; }
};
