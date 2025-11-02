#include <utility>
#ifndef COMMAND_DISPATCHER_H
#define COMMAND_DISPATCHER_H

#include <functional>
#include <map>

#include "../common/common_codes.h"

class CommandDispatcher {
private:
    std::map<ActionCode, std::function<void()>> handlers;

public:
    CommandDispatcher() = default;

    void registerHandler(ActionCode code, std::function<void()> fn) {
        handlers[code] = std::move(fn);
    }

    bool dispatch(ActionCode code) {
        auto it = handlers.find(code);
        if (it == handlers.end())
            return false;
        it->second();
        return true;
    }
};

#endif
