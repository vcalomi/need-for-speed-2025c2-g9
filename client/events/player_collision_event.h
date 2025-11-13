#pragma once
#include <string>
#include <utility>
#include <vector>

#include "../../common/common_codes.h"
#include "../world/checkpoint.h"

#include "event.h"

struct PlayerCollisionEvent: public Event {
    std::string player1_username;
    std::string player2_username;

    explicit PlayerCollisionEvent(std::string player1_username, std::string player2_username):
            player1_username(std::move(player1_username)),
            player2_username(std::move(player2_username)) {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "PlayerCollisionEvent"; }
};
