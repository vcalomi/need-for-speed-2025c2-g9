#pragma once
#include <string>
#include <utility>

#include "event.h"

struct UpgradeCarEvent: public Event {
    std::string username;
    bool upgradeHealth;
    bool upgradeSpeed;

    explicit UpgradeCarEvent(std::string username, bool upgradeHealth, bool upgradeSpeed):
            username(std::move(username)),
            upgradeHealth(upgradeHealth),
            upgradeSpeed(upgradeSpeed) {}

    std::string GetType() const override { return Type(); }
    static std::string Type() { return "UpgradeCarEvent"; }
};
