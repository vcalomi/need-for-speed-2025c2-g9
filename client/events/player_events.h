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

#pragma once

struct PlayerStateUpdatedEvent: public Event {
    std::string username;
    float x;
    float y;
    float angle;
    float speed;
    bool isAboveBridge;

    PlayerStateUpdatedEvent(std::string username, float x, float y, float angle, float speed,
                            bool isAboveBridge):
            username(std::move(username)),
            x(x),
            y(y),
            angle(angle),
            speed(speed),
            isAboveBridge(isAboveBridge) {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "PlayerStateUpdatedEvent"; }
};
