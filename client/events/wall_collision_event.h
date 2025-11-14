#pragma once
#include <string>
#include <utility>
#include <vector>

#include "../../common/common_codes.h"
#include "../world/checkpoint.h"

#include "event.h"

struct WallCollisionEvent: public Event {
    std::string player1_username;
    float player1_new_hp;

    explicit WallCollisionEvent(std::string player1_username, float player1_new_hp):
            player1_username(std::move(player1_username)), player1_new_hp(player1_new_hp) {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "WallCollisionEvent"; }
};
