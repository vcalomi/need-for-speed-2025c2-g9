#pragma once
#include <iostream>
#include <memory>

#include "../../common/Dto/dto.h"
#include "../../common/Dto/player.h"
#include "../../common/Dto/player_move.h"
#include "../../common/Dto/vehicle.h"
#include "../events/event.h"
#include "../network/client.h"
#include "../world/world.h"

class DtoHandlerSystem {
public:
    DtoHandlerSystem(World& world, Client& client, EventBus& eventBus);
    void Process(const std::shared_ptr<Dto>& dto);

private:
    World& world_;
    Client& client_;
    EventBus& eventBus_;
};
