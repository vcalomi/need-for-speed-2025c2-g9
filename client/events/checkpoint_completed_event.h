#pragma once
#include <string>
#include <utility>
#include <vector>

#include "../../common/common_codes.h"
#include "../world/checkpoint.h"

#include "event.h"

struct CheckPointCompletedEvent: public Event {
    std::string username;
    int checkpoint_id;

    explicit CheckPointCompletedEvent(std::string username, int checkpoint_id):
            username(std::move(username)), checkpoint_id(checkpoint_id) {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "CheckPointCompletedEvent"; }
};
