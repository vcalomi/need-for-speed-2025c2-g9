#pragma once
#include <string>
#include <utility>

#include "../../common/common_codes.h"

#include "event.h"

struct PlayerMoveEvent: public Event {
    std::string username;
    ActionCode move;

    PlayerMoveEvent(std::string username, ActionCode move):
            username(std::move(username)), move(move) {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "PlayerMoveEvent"; }
};
