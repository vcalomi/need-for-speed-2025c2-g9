#pragma once
#include <memory>
#include <unordered_map>

#include "../../common/Dto/dto.h"
#include "../../common/Dto/player.h"
#include "../../common/Dto/player_move.h"
#include "../../common/Dto/vehicle.h"
#include "../events/event.h"
#include "../network/client.h"
#include "../world/world.h"
#include "dto_handlers/dto_handler.h"

class DtoHandlerSystem {
public:
    DtoHandlerSystem(Client& client, EventBus& eventBus);
    void Process(const std::shared_ptr<Dto>& dto);

private:
    Client& client_;
    EventBus& eventBus_;
    std::unordered_map<uint8_t, std::unique_ptr<DtoHandler>> handlers_;

    void RegisterHandlers();
};
