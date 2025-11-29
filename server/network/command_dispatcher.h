#include <utility>
#ifndef COMMAND_DISPATCHER_H
#define COMMAND_DISPATCHER_H

#include <functional>
#include <map>
#include <memory>

#include "../../common/common_codes.h"
#include "../../common/Dto/dto.h"

class CommandDispatcher {
private:
    std::map<ActionCode, std::function<void(std::shared_ptr<Dto>)>> handlers;

public:
    CommandDispatcher() = default;

    void registerHandler(ActionCode code, std::function<void(std::shared_ptr<Dto>)> handler) {
        handlers[code] = std::move(handler);
    }

    bool dispatch(ActionCode code, std::shared_ptr<Dto> dto) {
        auto it = handlers.find(code);
        if (it == handlers.end()) {
            return false;
        }
        it->second(dto);
        return true;
    }
};

#endif
